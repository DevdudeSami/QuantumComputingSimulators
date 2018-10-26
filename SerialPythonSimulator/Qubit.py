import math
import random
import numpy as np
import cmath

# a|0> + b|1>, where a and b are complex numbers. |0> and |1> are basis vectors. |0> = (1,0), |1> = (0,1).
class Qubit:

  def __init__(self, a, b=None):

    if not b:
      if a.mag**2 > 1 or a.mag**2 < 0: raise ValueError("Probability error. Must be smaller than 1 and greater than 0.")
      self.amplitudes = np.array([a, 1+0j - cmath.sqrt(complex(a.mag**2))], dtype = complex)
      return

    # if not round(a.mag**2 + b.mag**2) == 1: raise ValueError("Probability error. Must add up to one. Actual value: " + str(round(a.mag**2 + b.mag**2)))

    unnormalisedAmplitudes = np.array([a, b], dtype = complex)
    self.amplitudes = unnormalisedAmplitudes/np.linalg.norm(unnormalisedAmplitudes)

  def __str__(self):
    return str(self.amplitudes[0]) + "|0> + " + str(self.amplitudes[1]) + "|1>"

  # @staticmethod
  # def fromComplex(r, i=0):
  #   return Qubit(Complex(r, i))

  @staticmethod
  def fromProbability(aProb):
    return Qubit(cmath.sqrt(aProb))

  # @property
  # def b(self):
  #   return Complex(1).subtractFromThis(Complex(self.a.mag**2)).sqrt

  @property
  def aProb(self):
    return cmath.polar(self.amplitudes[0])[0]**2

  @property
  def bProb(self):
    return cmath.polar(self.amplitudes[1])[0]**2

  @property
  def measure(self):
    randomValue = random.uniform(0,1)
    return 1 if randomValue > self.aProb else 0

# myQubit = Qubit.fromComplex(1/math.sqrt(2))
# myQubit = Qubit.fromProbability(0.1)

# for i in range(0,10):
#   print(myQubit.measure)