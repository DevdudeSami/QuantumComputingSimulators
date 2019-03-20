from MFQRegister import MFQRegister
import time
import math
import cmath
from Accumulator import takeMeasurements

X = [0j, 1+0j, 1+0j, 0j]
H = [1/math.sqrt(2)+0j, 1/math.sqrt(2)+0j, 1/math.sqrt(2)+0j, -1/math.sqrt(2)+0j]

def Rm(m):
	omega = cmath.exp(complex(2*cmath.pi)*complex(0,1)/complex(2**m))

	return [1,0,0,omega]

def QuantumFourierTransform(reg, qIDs):
	i=0
	while(i < len(qIDs)):
		reg.applySingleGate(qIDs[i], H)
		
		j=i+1
		while(j < len(qIDs)):
			reg.applyCnGate([qIDs[j]], qIDs[i], Rm(j-i+1))
			j+=1
		i+=1
	
start_time = time.time()

reg = MFQRegister(20)
QuantumFourierTransform(reg, list(range(reg.n)))

print("%s" % (time.time() - start_time), end="")
