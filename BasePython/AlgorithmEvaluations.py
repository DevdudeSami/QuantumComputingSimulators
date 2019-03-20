from QRegister import QRegister
from Accumulator import Accumulator
import numpy as np
import scipy.sparse as scsp
import time
import math

I = scsp.csr_matrix(np.array([[1,0],[0,1]], dtype=complex))
H = scsp.csr_matrix(np.array([[1,1],[1,-1]], dtype=complex)/math.sqrt(2))
X = scsp.csr_matrix(np.array([[0,1],[1,0]], dtype=complex))
Y = scsp.csr_matrix(np.array([[1j,0],[0,-1j]], dtype=complex))
Z = scsp.csr_matrix(np.array([[1,0],[0,-1]], dtype=complex))
P = scsp.csr_matrix(np.array([[1,0],[0,1j]], dtype=complex))
CNOT = scsp.csr_matrix(np.array([[1,0,0,0],[0,1,0,0],[0,0,0,1],[0,0,1,0]], dtype=complex))
TOFF = scsp.csr_matrix(np.array([[1,0,0,0,0,0,0,0],[0,1,0,0,0,0,0,0],[0,0,1,0,0,0,0,0],[0,0,0,1,0,0,0,0],[0,0,0,0,1,0,0,0],[0,0,0,0,0,1,0,0],[0,0,0,0,0,0,0,1],[0,0,0,0,0,0,1,0]], dtype=complex))
SWAP = scsp.csr_matrix(np.array([[1,0,0,0],[0,0,1,0],[0,1,0,0],[0,0,0,1]], dtype=complex))

start_time = time.time()

########## Cucarro 6-bit ripple carry adder

def sixBitRippleCarryAdderCircuit(comp, qIDs):
  assert(comp.n >= 14)
  assert(len(qIDs) == 14)

  # Time slice 1
  comp.applyNGate([qIDs[4], qIDs[3]], CNOT)
  comp.applyNGate([qIDs[6], qIDs[5]], CNOT)
  comp.applyNGate([qIDs[8], qIDs[7]], CNOT)
  comp.applyNGate([qIDs[10], qIDs[9]], CNOT)
  comp.applyNGate([qIDs[12], qIDs[11]], CNOT)
  print(1)

  # Time slice 2
  comp.applyNGate([qIDs[4], qIDs[2]], CNOT)
  print(2)
  
  # Time slice 3
  comp.applyNGate([qIDs[0], qIDs[1], qIDs[2]], TOFF)
  comp.applyNGate([qIDs[6], qIDs[4]], CNOT)
  print(3)
    
  # Time slice 4
  comp.applyNGate([qIDs[2], qIDs[3], qIDs[4]], TOFF)
  comp.applyNGate([qIDs[8], qIDs[6]], CNOT)
  print(4)
    
  # Time slice 5
  comp.applyNGate([qIDs[4], qIDs[5], qIDs[6]], TOFF)
  comp.applyNGate([qIDs[10], qIDs[8]], CNOT)
  print(5)
    
  # Time slice 6
  comp.applyNGate([qIDs[6], qIDs[7], qIDs[8]], TOFF)
  comp.applyNGate([qIDs[12], qIDs[10]], CNOT)
  print(6)
    
  # Time slice 7
  comp.applyNGate([qIDs[8], qIDs[9], qIDs[10]], TOFF)
  comp.applyNGate([qIDs[12], qIDs[13]], CNOT)
  print(7)
    
  # Time slice 8
  comp.flipQubit(3)
  comp.flipQubit(5)
  comp.flipQubit(7)
  comp.flipQubit(9)
  comp.applyNGate([qIDs[10], qIDs[11], qIDs[13]], TOFF)
  print(8)

  # Time slice 9
  comp.applyNGate([qIDs[2], qIDs[3]], CNOT)
  comp.applyNGate([qIDs[4], qIDs[5]], CNOT)
  comp.applyNGate([qIDs[6], qIDs[7]], CNOT)
  comp.applyNGate([qIDs[8], qIDs[9]], CNOT)
  comp.applyNGate([qIDs[10], qIDs[11]], CNOT)
  print(9)

  # Time slice 10
  comp.applyNGate([qIDs[8], qIDs[9], qIDs[10]], TOFF)
  print(10)

  # Time slice 11
  comp.applyNGate([qIDs[6], qIDs[7], qIDs[8]], TOFF)
  comp.flipQubit(9)
  comp.applyNGate([qIDs[12], qIDs[10]], CNOT)
  print(11)

  # Time slice 12
  comp.applyNGate([qIDs[4], qIDs[5], qIDs[6]], TOFF)
  comp.flipQubit(7)
  comp.applyNGate([qIDs[10], qIDs[8]], CNOT)
  print(12)

  # Time slice 13
  comp.applyNGate([qIDs[2], qIDs[3], qIDs[4]], TOFF)
  comp.flipQubit(5)
  comp.applyNGate([qIDs[8], qIDs[6]], CNOT)
  print(13)
  
  # Time slice 14
  comp.applyNGate([qIDs[0], qIDs[1], qIDs[2]], TOFF)
  comp.flipQubit(3)
  comp.applyNGate([qIDs[6], qIDs[4]], CNOT)
  print(14)

  # Time slice 15
  comp.applyNGate([qIDs[4], qIDs[2]], CNOT)
  print(15)

  # Time slice 16
  comp.applyNGate([qIDs[1], qIDs[0]], CNOT)
  comp.applyNGate([qIDs[4], qIDs[3]], CNOT)
  comp.applyNGate([qIDs[6], qIDs[5]], CNOT)
  comp.applyNGate([qIDs[8], qIDs[7]], CNOT)
  comp.applyNGate([qIDs[10], qIDs[9]], CNOT)
  comp.applyNGate([qIDs[12], qIDs[11]], CNOT)
  print(16)


def sixBitRippleCarryAdder(A, B):
  comp = QRegister.withQubits(14)

  # Bit 2 is the input carry

  # Bits 0,3,5,6,9,11 are B
  if(B[5] == '1'): comp.flipQubit(0)
  if(B[4] == '1'): comp.flipQubit(3)
  if(B[3] == '1'): comp.flipQubit(5)
  if(B[2] == '1'): comp.flipQubit(7)
  if(B[1] == '1'): comp.flipQubit(9)
  if(B[0] == '1'): comp.flipQubit(11)

  # Bits 1,4,6,8,10,12 are B
  if(A[5] == '1'): comp.flipQubit(1)
  if(A[4] == '1'): comp.flipQubit(4)
  if(A[3] == '1'): comp.flipQubit(6)
  if(A[2] == '1'): comp.flipQubit(8)
  if(A[1] == '1'): comp.flipQubit(10)
  if(A[0] == '1'): comp.flipQubit(12)

  # Bit 13 is the Z bit

  sixBitRippleCarryAdderCircuit(comp, comp.allQubits)

  m = comp.measure[0]
  return m[13] + m[11] + m[9] + m[7] + m[5] + m[3] + m[0]


print(sixBitRippleCarryAdder("111111", "111111"))

print("--- %s seconds ---" % (time.time() - start_time))
