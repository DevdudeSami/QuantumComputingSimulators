# from RegisterQubit import RegisterQubit
from Accumulator import Accumulator
from StateVector import StateVector as V
import numpy as np
import scipy.sparse as scsp
from functools import reduce
from itertools import product
import random
import math
import time

I = scsp.csr_matrix(np.array([[1,0],[0,1]], dtype=complex))
H = scsp.csr_matrix(np.array([[1,1],[1,-1]], dtype=complex)/math.sqrt(2))
X = scsp.csr_matrix(np.array([[0,1],[1,0]], dtype=complex))
Y = scsp.csr_matrix(np.array([[1j,0],[0,-1j]], dtype=complex))
Z = scsp.csr_matrix(np.array([[1,0],[0,-1]], dtype=complex))
P = scsp.csr_matrix(np.array([[1,0],[0,1j]], dtype=complex))
CNOT = scsp.csr_matrix(np.array([[1,0,0,0],[0,1,0,0],[0,0,0,1],[0,0,1,0]], dtype=complex))
TOFF = scsp.csr_matrix(np.array([[1,0,0,0,0,0,0,0],[0,1,0,0,0,0,0,0],[0,0,1,0,0,0,0,0],[0,0,0,1,0,0,0,0],[0,0,0,0,1,0,0,0],[0,0,0,0,0,1,0,0],[0,0,0,0,0,0,0,1],[0,0,0,0,0,0,1,0]], dtype=complex))
SWAP = scsp.csr_matrix(np.array([[1,0,0,0],[0,0,1,0],[0,1,0,0],[0,0,0,1]], dtype=complex))

def CU(G):
  return np.array([[1,0,0,0],[0,1,0,0],[0,0,G[0][0],G[0][1]],[0,0,G[1][0],G[1][1]]], dtype=complex)

# Gates for Deustch's Algorithm
DUf00 = np.kron(I,I)
DUf01 = CNOT.copy()
DUf10 = np.array([[0,1,0,0],[1,0,0,0],[0,0,1,0],[0,0,0,1]], dtype=complex)
DUf11 = np.array([[0,1,0,0],[1,0,0,0],[0,0,0,1],[0,0,1,0]], dtype=complex)

## Quantum Oracle Mats
Vf1 = I.copy()
Vf2 = X.copy()
Vf3 = np.array([[1,1],[0,1]], dtype=complex)
Vf4 = np.array([[1,1],[1,0]], dtype=complex)
Vf5 = np.array([[0,1],[1,1]], dtype=complex)
Vf6 = np.array([[1,0],[1,1]], dtype=complex)
Vfs = [Vf1, Vf2, Vf3, Vf4, Vf5, Vf6]



# Gates for quantum teleportation
L = np.array([[1,-1],[1,1]], dtype=complex)/math.sqrt(2)
R = np.array([[1,1],[-1,1]], dtype=complex)/math.sqrt(2)
S = np.array([[1j,0],[0,1]], dtype=complex)
T = np.array([[-1,0],[0,-1j]], dtype=complex)

class QRegister:

  def __init__(self, *vectors):
    self.n = reduce(lambda x, y: x + y, map(lambda v: len(v.registerIndices), vectors))
    self.vectors = list(vectors)
    
  @staticmethod
  def fromString(desc):
    vectors = []
    currentQubitIndex = 0
    for v in desc.split('|'):
      amplitudes = scsp.csr_matrix(np.array(list(map(lambda x: complex(x), v.split(',')))))
      n = int(math.log(amplitudes.shape[1], 2))
      vectors.append(V(amplitudes, list(range(currentQubitIndex, currentQubitIndex+n))))
      currentQubitIndex += n
    return QRegister(*vectors)

  @staticmethod
  def withQubits(n):
    vectors = []
    for i in range(0,n):
      amps = scsp.csr_matrix(np.array([1,0]))
      vectors.append(V(amps, [i]))
    return QRegister(*vectors)

  @property
  def stateVector(self):
    if len(self.vectors) == 1: return self.vectors[0].measure
    elif (len(self.vectors) >= 2): 
      stateVector = scsp.kron(self.vectors[0].amplitudes, self.vectors[1].amplitudes)
      for i in range(2, len(self.vectors)):
        stateVector = scsp.kron(stateVector, self.vectors[i].amplitudes)
      return stateVector
    return None

  def __str__(self):
    combinations = list(product('01', repeat=len(self.vectors)))

    states = map("".join, combinations)

    result = ""

    for (state, amplitude) in zip(states, self.stateVector):
      result += "(" + str(amplitude) + ")" + "|" + state + ">" + " + "
    result = result[:-3]

    return result

  @property
  def probabilities(self):
    return np.array(list(map(lambda x: abs(x)**2, self.stateVector)))

  @property
  def measure(self):

    resultState = [''] * self.n
    for vector in self.vectors:
      vectorMeasurements = str(vector.measure[0])
      for (measurement, index) in zip(vectorMeasurements, vector.registerIndices):
        resultState[index] = measurement

    return ''.join(resultState), "|" + ''.join(resultState) + ">"

  def measureQubit(self, qID):
    v = self.stateVectorWithQId(qID)
    requiredQIndex = v.registerIndices.index(qID)
    measurement = v.measure[1]
    return int(measurement[requiredQIndex+1])
    
  def flipQubit(self, qID):
    self.applySingleQGate(qID, X)

  def stateVectorWithQId(self, qID):
    for v in self.vectors:
      if qID in v.registerIndices: return v

  def listIndexFromQIndex(self, qIndex):
    for i, vector in enumerate(self.vectors):
      if qIndex in vector.registerIndices:
        return i

  def applySingleQGate(self, qIndex, G):
    self.vectors[self.listIndexFromQIndex(qIndex)].applyOddGate(G, [qIndex])

  def applySingleQGateToMultipleQubits(self, qIndices, G):
    for i in qIndices:
      self.applySingleQGate(i, G)

  def applyGateToCombinedState(self, qIndices, G):
    stateContainingRequiredQubits = list(filter(lambda vector: qIndices[0] in vector.registerIndices, self.vectors))[0]
    stateContainingRequiredQubits.applyOddGate(G, qIndices)

  def combineTwoQubits(self, q1Index, q2Index):
    # if already combined, do nothing
    if self.vectors.index(self.stateVectorWithQId(q1Index)) == self.vectors.index(self.stateVectorWithQId(q2Index)):
      return self.vectors.index(self.stateVectorWithQId(q1Index))

    q1ListIndex = self.listIndexFromQIndex(q1Index)
    q2ListIndex = self.listIndexFromQIndex(q2Index)

    combinedState = self.vectors[q1ListIndex].combineWith(self.vectors[q2ListIndex])
    self.vectors[min(q1ListIndex,q2ListIndex)] = combinedState
    del self.vectors[max(q1ListIndex,q2ListIndex)]
    return min(q1ListIndex,q2ListIndex)

  def combineQubits(self,indices):
    finalCombinedStateListIndex = None
    firstQID = indices[0]
    for i in indices:
      if i == firstQID: continue
      finalCombinedStateListIndex = self.combineTwoQubits(firstQID,i)
    return finalCombinedStateListIndex

  def applyNGate(self,indices,G):
    combinedStateListIndex = self.combineQubits(indices)
    self.vectors[combinedStateListIndex].applyNGate(G,indices)

  def applyDoubleQGateToSingleQubits(self, q1Index, q2Index, G):
    combinedStateListIndex = self.combineTwoQubits(q1Index, q2Index)
    self.vectors[combinedStateListIndex].applyDoubleQGate(G, q1Index, q2Index)

  # q1 is the control, q2 is the target
  def entangleTwoQubits(self, q1, q2):
    self.applySingleQGate(q1, H)
    self.applyNGate([q1, q2], CNOT)

  def entangleThreeQubits(self, q1, q2, q3):
    self.applySingleQGate(q1, H)
    self.applyDoubleQGateToSingleQubits(q1, q2, CNOT)
    self.applyDoubleQGateToSingleQubits(q1, q3, CNOT)

  def entangleNQubits(self, indices):
    self.applySingleQGate(indices[0], H)
    for i in range(1,len(indices)):
      self.applyDoubleQGateToSingleQubits(indices[0], indices[i], CNOT)

  @property
  def allQubits(self): return list(range(0,self.n))

start_time = time.time()

# stateDescription = '1,0|1,0'

# myReg = QRegister.fromString(stateDescription)
# myReg = QRegister.withQubits(4)

# myReg.applySingleQGate(0,X)
# myReg.applySingleQGate(1,X)
# print(myReg.vectors)
# myReg.applyNGate([0,1],CU(H))

# myReg.applySingleQGateToMultipleQubits([0,1],H)
# myReg.applySingleQGate(0,X)
# myReg.applyNGate([0,1,3],TOFF)
# myReg.applyNGate([0,1,2],TOFF)

# myReg.vectors[0].swap(2,1)
# print(myReg.vectors[0].registerIndices)
# myReg.vectors[0].swap(0,1)

# myReg.entangleTwoQubits(1,0)
# myReg.entangleTwoQubits(3,2)
# myReg.entangleTwoQubits(2,1)

# myReg.entangleThreeQubits(0,1,2)

# myReg.entangleNQubits(myReg.allQubits)

# myAcc = Accumulator(myReg)
# print(myAcc.takeMeasurements(1000))


######### Deutsch's Algorithm #########

def deutschAlgorithm(gate):
  initialState = "1,0|1,0"

  dReg = QRegister.fromString(initialState)

  dReg.applySingleQGate(1,X) # second qubit should be a 1 to start with
  dReg.applySingleQGate(0,H)
  dReg.applySingleQGate(1,H)

  dReg.applyDoubleQGateToSingleQubits(0,1,gate)
  dReg.applySingleQGate(0,H)

  m = dReg.measureQubit(0)
  

  return "constant" if m == 0 else "balanced"

# print(deutschAlgorithm(DUf00))
# print(deutschAlgorithm(DUf01))
# print(deutschAlgorithm(DUf10))
# print(deutschAlgorithm(DUf11))

######## DJ Algorithm #########

def DJAlgorithm(gate):
  n = int(math.log(gate.shape[0], 2))

  initialState = "1,0|" * n
  initialState = initialState[:-1]

  dReg = QRegister.fromString(initialState)

  dReg.applySingleQGate(n-1,X) # last qubit should be a 1 to start with
  
  for i in range(0,n):
    dReg.applySingleQGate(i,H)
  
  dReg.applyNGate(list(range(0,n)),gate)

  for i in range(0,n-1):
    dReg.applySingleQGate(i,H)
 
  measurements = dReg.measure[0][:-1]

  return "constant" if measurements == "0" * (n-1) else "balanced"

# print(DJAlgorithm(DUf00))
# print(DJAlgorithm(DUf01))
# print(DJAlgorithm(DUf10))
# print(DJAlgorithm(DUf11))

########## TELEPORTATION ###########§

# q0 is x, q1 is y, q2 is z
# initialStateDescription = "1,0|1,0|1,0" # x, y, z

# teleportationReg = QRegister.fromString(initialStateDescription)

# teleportationReg.applySingleQGate(1,L)
# teleportationReg.applyDoubleQGateToSingleQubits(1,2,CNOT)
# teleportationReg.applyDoubleQGateToSingleQubits(0,1,CNOT)
# teleportationReg.applySingleQGate(0,R)
# teleportationReg.applySingleQGate(0,S)
# teleportationReg.applyDoubleQGateToSingleQubits(1,2,CNOT)
# teleportationReg.applyDoubleQGateToSingleQubits(2,0,CNOT)
# teleportationReg.applySingleQGate(0,S)
# teleportationReg.applySingleQGate(2,T)
# teleportationReg.applyDoubleQGateToSingleQubits(2,0,CNOT)

# teleportationReg.entangleTwoQubits(1,0) # x and y start off entangled
# teleportationReg.applyDoubleQGateToSingleQubits(2,0,CNOT)
# teleportationReg.applySingleQGate(2,H)
# zMeasure = teleportationReg.measureQubit(2)
# xMeasure = teleportationReg.measureQubit(0)
# m = teleportationReg.measure[0]
# zMeasure = int(m[2])
# xMeasure = int(m[0])
# if xMeasure == 1: teleportationReg.applySingleQGate(1,X)
# if zMeasure == 1: teleportationReg.applySingleQGate(1,Z)

# print(teleportationReg.measureQubit(1))

# myAcc = Accumulator(teleportationReg)
# print(myAcc.takeMeasurements(1000))

######## Random Number Generator #######

def randomInt(n):
  quantumComputer = QRegister.withQubits(int(math.log(n, 2))+1)

  quantumComputer.applySingleQGateToMultipleQubits(quantumComputer.allQubits, H)

  m = quantumComputer.measure

  return int(m[0], 2)

def randomIntTester(n, counts):
  results = {}
  for _ in range(counts):
    rand = randomInt(n)
    if str(rand) in results: results[str(rand)] += 1
    else: results[str(rand)] = 1
  return results


# print(randomIntTester(3, 1000))


######### Half Adder ###########

def halfAdderCircuit(qComputer, qIDs=[0,1,2]):
  assert(qComputer.n >= 3)
  assert(len(qIDs) >= 3)

  qComputer.applyNGate(qIDs,TOFF)
  qComputer.applyDoubleQGateToSingleQubits(qIDs[0],qIDs[1],CNOT)

def halfAdder(A, B):
  qComputer = QRegister.withQubits(3)
  if A == 1: qComputer.flipQubit(0)
  if B == 1: qComputer.flipQubit(1)
  
  halfAdderCircuit(qComputer)

  m = qComputer.measure[0]
  return m[1], m[2]

print(halfAdder(0,0))
print(halfAdder(0,1))
print(halfAdder(1,0))
print(halfAdder(1,1))

######### Full Adder ###########

def fullAdderCircuit(qComputer, qIDs=[0,1,2,3]):
  assert(qComputer.n >= 4)
  assert(len(qIDs) >= 4)

  halfAdderCircuit(qComputer, [qIDs[0],qIDs[1],qIDs[3]])
  halfAdderCircuit(qComputer, [qIDs[1],qIDs[2],qIDs[3]])
  qComputer.applyNGate([0,1],CNOT)


def fullAdder(A, B, C = 0):
  qComputer = QRegister.withQubits(4)
  if A == 1: qComputer.flipQubit(0)
  if B == 1: qComputer.flipQubit(1)
  if C == 1: qComputer.flipQubit(2)

  fullAdderCircuit(qComputer)

  m = qComputer.measure[0]
  return m[3], m[2]

print(fullAdder(0,0,0))
print(fullAdder(0,0,1))
print(fullAdder(0,1,0))
print(fullAdder(0,1,1))
print(fullAdder(1,0,0))
print(fullAdder(1,0,1))
print(fullAdder(1,1,0))
print(fullAdder(1,1,1))

######### Ripple 4-bit Adder #########

# def ripple4BitAdderCircuit(qComputer, qIDs=[]):

# A is the first 4-bit number, B is the second 4-bit number
def ripple4BitAdder(A="0000", B="0000"):
  # need a qubit for each input - 8
  # need a qubit for each output - 4
  # need a qubit for the carry - 1

  qComputer = QRegister.withQubits(13)

  # let carry be the last qubit (id 12)
  # 0,2,4,6 are the A inputs
  # 1,3,5,7 are the B inputs
  # 8,9,10,11 are the S outputs

  # flip depending on A and B
  for i in range(0,len(A)):
    if A[i] == "1": qComputer.flipQubit(i*2)
  for i in range(0,len(B)):
    if B[i] == "1": qComputer.flipQubit(i*2+1)

  fullAdderCircuit(qComputer, [0,1,12,8])
  print("1")
  fullAdderCircuit(qComputer, [2,3,12,9])
  print("2")
  fullAdderCircuit(qComputer, [4,5,12,10])
  print("3")
  fullAdderCircuit(qComputer, [6,7,12,11])
  print("4")

  m = qComputer.measure[0]
  return m[8] + m[9] + m[10] + m[11]

# print(ripple4BitAdder("0000", "0010"))

print("--- %s seconds ---" % (time.time() - start_time))