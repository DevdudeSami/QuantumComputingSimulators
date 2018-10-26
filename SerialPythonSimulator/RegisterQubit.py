import numpy as np
import cmath
import random

class RegisterQubit:

  def __init__(self, a, b):

    if not round(abs(a) + abs(b)) == 1: raise ValueError("Probability error. Must add up to one. Actual value: " + str(round(abs(a) + abs(b))))

    self.entangled = False
    self.amplitudes = np.array([a,b], dtype=complex)

  @property
  def probabilities(self):
    assert(not self.entangled)
    return abs(self.amplitudes[0]), abs(self.amplitudes[1])

  @property
  def measure(self):
    aProb, _ = self.probabilities
    rand = random.uniform(0,1)
    if rand > aProb: return 0
    return 1

  def __repr__(self):
    return str(self.amplitudes[0]) + "|0> + " + str(self.amplitudes[1]) + "|1>"






