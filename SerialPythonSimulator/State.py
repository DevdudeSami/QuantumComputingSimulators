import math
from functools import reduce
from itertools import product
from cmath import *
import numpy as np
import random

class State:

  def __init__(self, n, amplitudes):
    if len(amplitudes) > 2**n: raise AssertionError("Length of list amplitudes greater than number of qubits squared.")
    
    # normalisationCheck = reduce((lambda x, y: (x + polar(y)[0]**2)), amplitudes, 0)
    # if round(normalisationCheck, 10) != 1: raise AssertionError("Quantum state not normalised.")

    missingZeroCount = 2**n - len(amplitudes)
    newAmplitudes = amplitudes
    for _ in range(0,missingZeroCount): newAmplitudes.append(0)

    self.n = n
    self.amplitudes = np.array(newAmplitudes) / np.linalg.norm(newAmplitudes)


  def __str__(self):
    combinations = list(product('01', repeat=self.n))

    states = map("".join, combinations)

    result = ""

    for (state, amplitude) in zip(states, self.amplitudes):
      result += "(" + str(amplitude) + ")" + "|" + state + ">" + " + "
    result = result[:-3]

    return result

  @property
  def probabilities(self):
    return np.array(list(map(lambda x: polar(x)[0]**2, self.amplitudes)))

  @property
  def measure(self):
    combinations = list(product('01', repeat=self.n))
    states = list(map("".join, combinations))

    return "|" + random.choices(population=states, weights=self.probabilities, k=1)[0] + ">"

  def normalise(self):
    norm = np.linalg.norm(self.amplitudes)
    self.amplitudes /= norm

    

# print(State(3, [0,(1+1j)/math.sqrt(2)]))

