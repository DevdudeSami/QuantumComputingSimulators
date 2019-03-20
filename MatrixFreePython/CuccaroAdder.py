from MFQRegister import MFQRegister
import time

X = [0j, 1+0j, 1+0j, 0j]

def NBitCuccaroAdderCircuit(n, reg, qIDs):
	N = 2*n+2
	assert(len(qIDs) == N)

	# Step 1
	i = 3
	while(i < N-2):
		reg.applyCnGate([qIDs[i+1]], qIDs[i], X)
		i+=2

	# Step 2
	i = 0
	while(i < N-5):
		reg.applyCnGate([qIDs[i+4]], qIDs[i+2], X)
		reg.applyCnGate([qIDs[i], qIDs[i+1]], qIDs[i+2], X)
		i+=2

	# Step 3
	reg.applyCnGate([qIDs[N-2]], qIDs[N-1], X)

	# Step 4
	i = 3
	while(i<N-4):
		reg.applySingleGate(qIDs[i], X)
		i+=2

	# Step 5
	reg.applyCnGate([qIDs[N-4], qIDs[N-3]], qIDs[N-1], X)

	# Step 6
	i = 2
	while(i < N-3):
		reg.applyCnGate([qIDs[i]], qIDs[i+1], X)
		i+=2

	# Step 7
	reg.applyCnGate([qIDs[N-6], qIDs[N-5]], qIDs[N-4], X)

	# Step 8
	i = N-8
	while(i>=0):
		reg.applyCnGate([qIDs[i], qIDs[i+1]], qIDs[i+2], X)
		reg.applySingleGate(qIDs[i+3], X)
		reg.applyCnGate([qIDs[i+6]], qIDs[i+4], X)
		i-=2

	# Steps 9 and 10
	reg.applyCnGate([qIDs[4]], qIDs[2], X)
	reg.applyCnGate([qIDs[1]], qIDs[0], X)

	# Step 11
	i = 3
	while(i < N-2):
		reg.applyCnGate([qIDs[i+1]], qIDs[i], X)
		i+=2

def NBitCuccaroAdder(n, A, B):
	assert(len(A) == n)
	assert(len(B) == n)

	N = 2*n+2

	reg = MFQRegister(N)

	# Bit 2 is the input carry
  
  # Bits 0,3,5,7,9,11,..,N-3 are B
  # Bits 1,4,6,8,10,12,..,N-2 are A

	if B[n-1] == '1': reg.applySingleGate(0, X)
	if A[n-1] == '1': reg.applySingleGate(1, X)

	i = 0
	while(i<n-1):
		if B[i] == '1': reg.applySingleGate((N-3) - i*2, X)
		if A[i] == '1': reg.applySingleGate((N-2) - i*2, X)
		i+=1

	# The last bit is the Z bit

	NBitCuccaroAdderCircuit(n, reg, list(range(N)))

	m = reg.measure()
	result = []


	i = N-1
	while(i >= 3):
		result.append(m[i])
		i-=2

	result.append(m[0])

	return "".join(result)

start_time = time.time()

NBitCuccaroAdder(8, "11111111", "11111111")

print("%s" % (time.time() - start_time))
