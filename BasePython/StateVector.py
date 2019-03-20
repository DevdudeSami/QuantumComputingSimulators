import math
import numpy as np
import scipy.sparse as scsp
from functools import reduce
from itertools import product
import random
import math
from Accumulator import Accumulator

I = scsp.csr_matrix(np.array([[1,0],[0,1]], dtype=complex))
H = scsp.csr_matrix(np.array([[1,1],[1,-1]], dtype=complex)/math.sqrt(2))
X = scsp.csr_matrix(np.array([[0,1],[1,0]], dtype=complex))
Y = scsp.csr_matrix(np.array([[1j,0],[0,-1j]], dtype=complex))
Z = scsp.csr_matrix(np.array([[1,0],[0,-1]], dtype=complex))
P = scsp.csr_matrix(np.array([[1,0],[0,1j]], dtype=complex))
CNOT = scsp.csr_matrix(np.array([[1,0,0,0],[0,1,0,0],[0,0,0,1],[0,0,1,0]], dtype=complex))
TOFF = scsp.csr_matrix(np.array([[1,0,0,0,0,0,0,0],[0,1,0,0,0,0,0,0],[0,0,1,0,0,0,0,0],[0,0,0,1,0,0,0,0],[0,0,0,0,1,0,0,0],[0,0,0,0,0,1,0,0],[0,0,0,0,0,0,0,1],[0,0,0,0,0,0,1,0]], dtype=complex))
SWAP = scsp.csr_matrix(np.array([[1,0,0,0],[0,0,1,0],[0,1,0,0],[0,0,0,1]], dtype=complex))

class StateVector:

  # amplitudes needs to be a csr matrix
  def __init__(self, amplitudes, registerIndices):
    # if(type(amplitudes) != scsp.bsr.bsr_matrix):
    #   amplitudes = scsp.csr_matrix(np.array(amplitudes, dtype=complex))
    # else:
    #   amplitudes = amplitudes.tocsr()

    assert(type(amplitudes) == scsp.csr_matrix)

    n = int(math.log(amplitudes.shape[1], 2))
    # if len(amplitudes) != 2**n: raise AssertionError("Length of list amplitudes not equal to number of qubits squared.")
    # assert(len(amplitudes) == 2**n)
    assert(len(registerIndices) == n)

    normalisationCheck = reduce((lambda x, y: (x + abs(y)**2)), amplitudes.data, 0)
    assert(round(normalisationCheck, 10) == 1)
    # if round(normalisationCheck, 10) != 1: raise AssertionError("Quantum state not normalised.")
 
    self.n = n
    self.amplitudes = amplitudes
    self.registerIndices = registerIndices


  def __repr__(self):
    combinations = list(product('01', repeat=self.n))

    states = map("".join, combinations)

    result = ""

    for (state, amplitude) in zip(states, self.amplitudes):
      result += "(" + str(amplitude) + ")" + "|" + state + ">" + " + "
    result = result[:-3]

    return result

  @property
  def probabilities(self):
    return np.array(list(map(lambda x: abs(x)**2, self.amplitudes.todense().tolist()[0])))

  @property
  def measure(self):
    combinations = list(product('01', repeat=self.n))
    states = list(map("".join, combinations))

    result = random.choices(population=states, weights=self.probabilities, k=1)[0]  

    return result, "|" + result + ">"

  # def normalise(self):
  #   norm = np.linalg.norm(self.amplitudes)
  #   self.amplitudes /= norm

  def applyGate(self, G):
    self.amplitudes = (G @ self.amplitudes.transpose()).transpose()

  def combineWith(self, v):
    newAmplitudes = scsp.kron(self.amplitudes, v.amplitudes).tocsr()
    return StateVector(newAmplitudes, self.registerIndices + v.registerIndices)

  @staticmethod
  def prepareMatrixOperator(O,r,n):
    if 0 in r: matrixOperator = O
    else: matrixOperator = I
    for i in range(1,n):
      if i in r: matrixOperator = scsp.kron(matrixOperator, O)
      else: matrixOperator = scsp.kron(matrixOperator, I)

    return matrixOperator

  # r1 and r2 are the indices in registerIndices, not the ids of the qubits
  def prepareDoubleQubitMatrixOperator(self,O,r1,r2):
    assert(r2 == r1 + 1)
    assert(O.shape == (4,4))

    if r1 == 0: 
      matrixOperator = O
      for i in range(2,self.n): matrixOperator = scsp.kron(matrixOperator, I)
    else: 
      matrixOperator = I
      for i in range(1,self.n): 
        if i == r1: matrixOperator = scsp.kron(matrixOperator, O)
        elif i == r2: pass
        else: matrixOperator = scsp.kron(matrixOperator, I)

    return matrixOperator
  
  # indices is the indices in registerIndices (in the state vector), not the ids of the qubits
  def prepareNQubitMatrixOperator(self,O,indices):
    for i in range(1,len(indices)): assert(indices[i] == indices[i-1] + 1)

    if indices[0] == 0: 
      matrixOperator = O
      for i in range(len(indices),self.n): matrixOperator = scsp.kron(matrixOperator, I)
    else: 
      matrixOperator = I
      for i in range(1,self.n): 
        if i == indices[0]: matrixOperator = scsp.kron(matrixOperator, O)
        elif i in indices: pass
        else: matrixOperator = scsp.kron(matrixOperator, I)

    return matrixOperator

  def addQubit(self):
    self.amplitudes = scsp.kron(self.amplitudes, np.array([1,0],dtype=complex))
    newQID = max(self.registerIndices)+1
    self.registerIndices += [newQID]
    self.n += 1
    
    # returns id of new qubit
    return newQID

  def applyNGate(self,G,qIDs):
    swapsDone = []
    qIndicesToSwapInto = list(range(len(qIDs)))
    
    for i in qIndicesToSwapInto:
      swapsDone += [(qIDs[i], self.registerIndices[i])]
      self.swap(qIDs[i], self.registerIndices[i])

    operator = self.prepareNQubitMatrixOperator(G,qIndicesToSwapInto)
    self.applyGate(operator)

    # swap back in the reverse order
    for swapDone in list(reversed(swapsDone)):
      self.swap(swapDone[0], swapDone[1])

  def swap(self, q1Index, q2Index):
    if(q1Index == q2Index): return

    desiredQ1Position = self.registerIndices.index(q2Index)
    desiredQ2Position = self.registerIndices.index(q1Index)

    steps = abs(desiredQ2Position - desiredQ1Position)
    
    q1PositionInVector= self.registerIndices.index(q1Index)
    q2PositionInVector= self.registerIndices.index(q2Index)

    if q1PositionInVector > q2PositionInVector: 
      q1Index, q2Index = q2Index, q1Index
      q1PositionInVector, q2PositionInVector = q2PositionInVector, q1PositionInVector

    i = q1PositionInVector
    while i < q1PositionInVector + steps:
      operator = self.prepareDoubleQubitMatrixOperator(SWAP, i, i+1)
      self.amplitudes = (operator @ self.amplitudes.transpose()).transpose()
      i += 1

    i = q2PositionInVector - 1
    while i > q2PositionInVector - steps:
      operator = self.prepareDoubleQubitMatrixOperator(SWAP, i-1, i)
      self.amplitudes = (operator @ self.amplitudes.transpose()).transpose()
      i -= 1

  def applyDoubleQGate(self, G, r1, r2):
    assert(self.n >= 2)
    assert(r1 in self.registerIndices)
    assert(r2 in self.registerIndices)

    r1Position = self.registerIndices.index(r1)
    r2Position = self.registerIndices.index(r2)

    if r2Position != r1Position + 1:

      if r1Position == len(self.registerIndices)-1:
        if r2Position == 0:
          self.swap(r1, r2)
          self.swap(self.registerIndices[1], r1)
          operator = self.prepareDoubleQubitMatrixOperator(G, 0, 1)
          self.amplitudes = (operator @ self.amplitudes.transpose()).transpose()
          self.swap(self.registerIndices[1], r1)
          self.swap(r1, r2)
          return

        self.swap(r1, self.registerIndices[r2Position-1])
        operator = self.prepareDoubleQubitMatrixOperator(G, r2Position-1, r2Position)
        self.amplitudes = (operator @ self.amplitudes.transpose()).transpose()
        self.swap(r1, self.registerIndices[r2Position-1])
        return
        

      self.swap(r2, self.registerIndices[r1Position+1])
      operator = self.prepareDoubleQubitMatrixOperator(G, r1Position, r1Position+1)
      self.amplitudes = (operator @ self.amplitudes.transpose()).transpose()
      self.swap(r2, self.registerIndices[r1Position+1])
    else:
      operator = self.prepareDoubleQubitMatrixOperator(G, r1Position, r1Position+1)
      self.amplitudes = (operator @ self.amplitudes.transpose()).transpose()


    
    

  #   subAmps = np.array([self.amplitudes[2*r[0]], self.amplitudes[2*r[0]+1], self.amplitudes[2*r[1]], self.amplitudes[2*r[1]+1]])
  #   newSubAmplitudes = G @ subAmps

  #   self.amplitudes[2*r[0]] = newSubAmplitudes[0]
  #   self.amplitudes[2*r[0]+1] = newSubAmplitudes[1]
  #   self.amplitudes[2*r[1]] = newSubAmplitudes[2]
  #   self.amplitudes[2*r[1]+1] = newSubAmplitudes[3]

  def applyOddGate(self, G, r=[0]):
    indices = list(map(lambda x: self.registerIndices.index(x), r))
    operator = StateVector.prepareMatrixOperator(G,indices,self.n)
    self.amplitudes = (operator @ self.amplitudes.transpose()).transpose()



# State vector testing

# st = StateVector(scsp.csr_matrix(np.array([1,0])), [0])
# st.applyGate(H)

# acc = Accumulator(st)

# print(acc.takeMeasurements(1000))
