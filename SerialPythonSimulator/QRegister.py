# from RegisterQubit import RegisterQubit
from Accumulator import Accumulator
from StateVector import StateVector as V
import numpy as np
from functools import reduce
from itertools import product
import random
import math

I = np.array([[1,0],[0,1]], dtype=complex)
H = np.array([[1,1],[1,-1]], dtype=complex)/math.sqrt(2)
X = np.array([[0,1],[1,0]], dtype=complex)
Y = np.array([[1j,0],[0,-1j]], dtype=complex)
Z = np.array([[1,0],[0,-1]], dtype=complex)
P = np.array([[1,0],[0,1j]], dtype=complex)
CNOT = np.array([[1,0,0,0],[0,1,0,0],[0,0,0,1],[0,0,1,0]], dtype=complex)
SWAP = np.array([[1,0,0,0],[0,0,1,0],[0,1,0,0],[0,0,0,1]], dtype=complex)

class QRegister:

  def __init__(self, *vectors):
    self.n = reduce(lambda x, y: x + y, map(lambda v: len(v.registerIndices), vectors))
    self.vectors = list(vectors)
    
  @staticmethod
  def fromString(desc):
    vectors = []
    currentQubitIndex = 0
    for v in desc.split('|'):
      amplitudes = list(map(lambda x: complex(x), v.split(',')))
      n = int(math.log(len(amplitudes), 2))
      vectors.append(V(amplitudes, list(range(currentQubitIndex, currentQubitIndex+n))))
      currentQubitIndex += n
    return QRegister(*vectors)

  @property
  def stateVector(self):
    if len(self.vectors) == 1: return self.vectors[0].measure
    elif (len(self.vectors) >= 2): 
      stateVector = np.kron(self.vectors[0].amplitudes, self.vectors[1].amplitudes)
      for i in range(2, len(self.vectors)):
        stateVector = np.kron(stateVector, self.vectors[i].amplitudes)
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

    return "|" + ''.join(resultState) + ">"

  def listIndexFromQIndex(self, qIndex):
    for i, vector in enumerate(self.vectors):
      if qIndex in vector.registerIndices:
        return i

  def applySingleQGate(self, qIndex, G):
    self.vectors[self.listIndexFromQIndex(qIndex)].applyGate(G)

  def applySingleQGateToMultipleQubits(self, qIndices, G):
    for i in qIndices:
      self.applySingleQGate(i, G)

  def applyGateToCombinedState(self, qIndices, G):
    stateContainingRequiredQubits = list(filter(lambda vector: qIndices[0] in vector.registerIndices, self.vectors))[0]
    stateContainingRequiredQubits.applyOddGate(G, qIndices)

  def combineTwoQubits(self, q1Index, q2Index):
    q1ListIndex = self.listIndexFromQIndex(q1Index)
    q2ListIndex = self.listIndexFromQIndex(q2Index)

    combinedState = self.vectors[q1ListIndex].combineWith(self.vectors[q2ListIndex])
    self.vectors[min(q1ListIndex,q2ListIndex)] = combinedState
    self.vectors = np.delete(self.vectors, max(q1ListIndex,q2ListIndex))
    return min(q1ListIndex,q2ListIndex)

  def applyDoubleQGateToSingleQubits(self, q1Index, q2Index, G):
    combinedStateListIndex = self.combineTwoQubits(q1Index, q2Index)
    self.vectors[combinedStateListIndex].applyGate(G)

  # q1 is the control, q2 is the target
  def entangleTwoQubits(self, q1, q2):
    self.applySingleQGate(q1, H)
    self.applyDoubleQGateToSingleQubits(q1, q2, CNOT)

stateDescription = '1,0|1,0'

# myReg = QRegister(V([1,0],[0]), V([1,0],[1]), V([1,0], [2]), V([0,1],[3]))
myReg = QRegister.fromString(stateDescription)

myReg.entangleTwoQubits(1,0)

# print(np.kron(H,I) @ myReg.vectors[0].amplitudes)

myReg.applyGateToCombinedState([0], H)
# print(myReg.vectors)

myAcc = Accumulator(myReg)
print(myAcc.takeMeasurements(1000))