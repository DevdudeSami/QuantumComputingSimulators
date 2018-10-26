import math
from State import State
from Complex import Complex
import numpy as np
from Accumulator import Accumulator

I = np.array([[1,0],[0,1]], dtype=complex)
H = np.array([[1,1],[1,-1]], dtype=complex)/math.sqrt(2)
X = np.array([[0,1],[1,0]], dtype=complex)
Y = np.array([[1j,0],[0,-1j]], dtype=complex)
Z = np.array([[1,0],[0,-1]], dtype=complex)
P = np.array([[1,0],[0,1j]], dtype=complex)
CNOT = np.array([[1,0,0,0],[0,1,0,0],[0,0,0,1],[0,0,1,0]], dtype=complex)

class Gates: 

  @staticmethod
  def h(state, r=[0]):
    matrixOperator = Gates.prepareMatrixOperator(H,r,state.n)
    return State(state.n, list(matrixOperator @ state.amplitudes))

  @staticmethod
  def x(state, r=[0]):
    matrixOperator = Gates.prepareMatrixOperator(X,r,state.n)
    return State(state.n, list(matrixOperator @ state.amplitudes))

  @staticmethod
  def y(state, r=[0]):
    matrixOperator = Gates.prepareMatrixOperator(Y,r,state.n)
    return State(state.n, list(matrixOperator @ state.amplitudes))

  @staticmethod
  def z(state, r=[0]):
    matrixOperator = Gates.prepareMatrixOperator(Z,r,state.n)
    return State(state.n, list(matrixOperator @ state.amplitudes))

  @staticmethod
  def p(state, r=[0]):
    matrixOperator = Gates.prepareMatrixOperator(P,r,state.n)
    return State(state.n, list(matrixOperator @ state.amplitudes))

  @staticmethod
  def cnot(state, r=[0,1]):
    assert(len(r) == 2)

    substate = State(2,[state.amplitudes[2*r[0]], state.amplitudes[2*r[0]+1], state.amplitudes[2*r[1]], state.amplitudes[2*r[1]+1]])
    newSubAmplitudes = CNOT @ substate.amplitudes
    newAmplitudes = state.amplitudes
    newAmplitudes[2*r[0]] = newSubAmplitudes[0]
    newAmplitudes[2*r[0]+1] = newSubAmplitudes[1]
    newAmplitudes[2*r[1]] = newSubAmplitudes[2]
    newAmplitudes[2*r[1]+1] = newSubAmplitudes[3]

    return State(state.n, newAmplitudes)

  @staticmethod
  def entangle(state, r=[0,1]):
    assert(len(r) == 2)

    hadamardState = Gates.h(state, [r[1]])
    return Gates.cnot(hadamardState, r)


  @staticmethod
  def prepareMatrixOperator(O,r,n):
    if 0 in r: matrixOperator = O
    else: matrixOperator = I
    for i in range(1,n):
      if i in r: matrixOperator = np.kron(matrixOperator, O)
      else: matrixOperator = np.kron(matrixOperator, I)

    return matrixOperator

# myState = Gates.h(State(4,[0,1,0,0]),[0,1,2,3])
# print(myState.probabilities)
# myAccumulator = Accumulator(myState)
# print(myAccumulator.takeMeasurements(100000))

# myState = State(2,[0,0,1,0])
# print(Gates.cnot(myState, [0,1]))

myState = State(2,[1])
print(Gates.entangle(myState, [1,0]))

# myState = State(2,[1])
# print(myState)
# print(Gates.h(myState, [0,1]))


print(Gates.h(State(2, [1]), [0]))

