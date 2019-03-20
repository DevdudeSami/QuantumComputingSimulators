from MFQRegister import MFQRegister
import time
import math
import cmath

X = [0j, 1+0j, 1+0j, 0j]
H = [1/math.sqrt(2)+0j, 1/math.sqrt(2)+0j, 1/math.sqrt(2)+0j, -1/math.sqrt(2)+0j]
Z = [1+0j, 0j, 0j, -1+0j]

def GroversOracle(reg, markedStates):
	for s in markedStates:
		reg.applySingleGate(s, X)

	# list of controls
	allQs = list(range(reg.n))
	controls = allQs[:-1]

	reg.applyCnGate(controls, reg.n-1, X)

	for s in markedStates:
		reg.applySingleGate(s, X)

def GroversDiffusion(reg):
	for i in range(reg.n-1):
		reg.applySingleGate(i, H)
		reg.applySingleGate(i, X)

	allQs = list(range(reg.n))
	controls = allQs[:-2]

	reg.applyCnGate(controls, reg.n-2, Z)

	for i in range(reg.n-1):
		reg.applySingleGate(i, X)
		reg.applySingleGate(i, H)

def GroversSearch(n, markedStates, iters):
	reg = MFQRegister(n+1)

	reg.applySingleGate(n, X)

	for i in range(n+1):
		reg.applySingleGate(i, H)

	for i in range(iters):
		GroversOracle(reg, markedStates)
		GroversDiffusion(reg)

	m = reg.measure()
	return m[:n]

start_time = time.time()

GroversSearch(10, [2,5,8], 32)

print("%s" % (time.time() - start_time), end="")
