import math

class Complex:

  def __init__(self, real, imag=0):
    self.r = real
    self.i = imag

  @staticmethod 
  def fromModAndArg(mod, arg): return Complex(mod*math.cos(arg), mod*math.sin(arg))

  def __str__(self):
    return str(self.r) + " + " + str(self.i) + "i"

  @property
  def conjugate(self): return Complex(self.r, -self.i)

  @property
  def mag(self): return math.sqrt(self.r**2 + self.i**2)

  @property
  def arg(self): return math.atan2(self.i, self.r)

  @property
  def negative(self): return Complex(-self.r, -self.i)

  @property
  def sqrt(self): return Complex.pow(self, 0.5)

  @staticmethod
  def add(c1, c2): return Complex(c1.r+c2.r, c1.i+c2.i)

  @staticmethod
  def subtract(c1, c2): return Complex.add(c1, c2.negative)

  @staticmethod
  def multiply(c1, c2):
    a, b, c, d = c1.r, c1.i, c2.r, c2.i
    return Complex(a*c-b*d, a*d+b*c)

  @staticmethod 
  def multiplyComplexByScalar(c, k): return Complex(c.r*k, c.i*k)

  @staticmethod
  def divideComplexByScalar(c, k): return Complex.multiplyByScalar(c, 1/k)

  @staticmethod
  def divide(c1, c2):
    a, b, c, d = c1.r, c1.i, c2.r, c2.i
    return Complex.divideComplexByScalar(Complex(a*c+b*d, b*c-a*d), c**2+d**2)

  @staticmethod
  def pow(c, k):
    r, theta = c.mag, c.arg
    return Complex((r**k)*math.cos(k*theta), (r**k)*math.sin(k*theta))

  @staticmethod
  def unitImaginary():
    return Complex(0,1)

  def addTo(self, c): return Complex.add(self, c)
  def subtractFromThis(self, c): return Complex.subtract(self, c)
  def subtractFrom(self, c): return Complex.subtract(c, self)
  def multiplyByComplex(self, c): return Complex.multiply(self, c)
  def multiplyByScalar(self, k): return Complex.multiplyComplexByScalar(self, k)
  def divideByComplex(self, c): return Complex.divide(self, c)
  def divideComplexByThis(self, c): return Complex.divide(c, self)
  def divideByScalar(self, k): return Complex.divideComplexByScalar(self, k)
