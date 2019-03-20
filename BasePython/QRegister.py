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