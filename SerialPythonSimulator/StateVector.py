import math
import numpy as np
from functools import reduce
from itertools import product
import random
import math

I = np.array([[1,0],[0,1]], dtype=complex)

class StateVector:

  def __init__(self, amplitudes, registerIndices):
    n = int(math.log(len(amplitudes), 2))

    # if len(amplitudes) != 2**n: raise AssertionError("Length of list amplitudes not equal to number of qubits squared.")
    # assert(len(amplitudes) == 2**n)
    assert(len(registerIndices) == n)
    
    normalisationCheck = reduce((lambda x, y: (x + abs(y)**2)), amplitudes, 0)
    assert(round(normalisationCheck, 10) == 1)
    # if round(normalisationCheck, 10) != 1: raise AssertionError("Quantum state not normalised.")

    self.n = n
    self.amplitudes = np.array(amplitudes, dtype=complex)
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
    return np.array(list(map(lambda x: abs(x)**2, self.amplitudes)))

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
    self.amplitudes = np.matmul(G, self.amplitudes)

  def combineWith(self, v):
    newAmplitudes = np.kron(self.amplitudes, v.amplitudes)
    return StateVector(newAmplitudes, self.registerIndices + v.registerIndices)

  @staticmethod
  def prepareMatrixOperator(O,r,n):
    if 0 in r: matrixOperator = O
    else: matrixOperator = I
    for i in range(1,n):
      if i in r: matrixOperator = np.kron(matrixOperator, O)
      else: matrixOperator = np.kron(matrixOperator, I)

    return matrixOperator

  def applyOddGate(self, G, r=[0]):
    indices = list(map(lambda x: self.registerIndices.index(x), r))
    operator = StateVector.prepareMatrixOperator(G,indices,self.n)
    self.amplitudes = np.matmul(operator, self.amplitudes)
