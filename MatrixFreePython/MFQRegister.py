from math import sqrt
from itertools import product
import random

def nth_cleared(n, t):
	mask = (1 << t) - 1
	not_mask = ~mask

	return (n & mask) | ((n & not_mask) << 1)

class MFQRegister:

	def __init__(self, n):
		self.n = n
		self.stateVector = [0j] * (2**n)
		self.stateVector[0] = 1+0j
	
	def applySingleGate(self, qID, gate):
		assert(len(gate) == 4)

		for i in range(0, 2**(self.n-1)):
			zero_state = nth_cleared(i, qID)
			one_state = zero_state | (1 << qID)
			zero_amp = self.stateVector[zero_state]
			one_amp = self.stateVector[one_state]

			self.stateVector[zero_state] = gate[0]*zero_amp + gate[1]*one_amp
			self.stateVector[one_state] = gate[2]*zero_amp + gate[3]*one_amp

	def applyControlledGate(self, controlQID, targetQID, gate):
		assert(len(gate) == 4)

		for i in range(0, 2**(self.n-1)):
			zero_state = nth_cleared(i, targetQID)
			one_state = zero_state | (1 << targetQID)

			control_val_zero = 1 if (((1 << controlQID) & zero_state) > 0) else 0
			control_val_one = 1 if (((1 << controlQID) & one_state) > 0) else 0

			zero_amp = self.stateVector[zero_state]
			one_amp = self.stateVector[one_state]

			if(control_val_zero == 1): self.stateVector[zero_state] = gate[0]*zero_amp + gate[1]*one_amp
			if(control_val_one): self.stateVector[one_state] = gate[2]*zero_amp + gate[3]*one_amp

	def applyCnGate(self, controls, targetQID, gate):
		assert(len(gate) == 4)

		for i in range(0, 2**(self.n-1)):
			zero_state = nth_cleared(i, targetQID)
			one_state = zero_state | (1 << targetQID)

			control_zero = True
			control_one = True

			for c in range(0, len(controls)):
				if(not (((1 << controls[c]) & zero_state) > 0)): control_zero = False
				if(not (((1 << controls[c]) & one_state) > 0)): control_one = False
				
				if(not control_zero and not control_one): break

			zero_amp = self.stateVector[zero_state]
			one_amp = self.stateVector[one_state]

			if(control_zero): self.stateVector[zero_state] = gate[0]*zero_amp + gate[1]*one_amp
			if(control_one): self.stateVector[one_state] = gate[2]*zero_amp + gate[3]*one_amp

	def measure(self):
		probs = []

		for amp in self.stateVector:
			probs.append(abs(amp)**2)

		combinations = list(product('01', repeat=self.n))
		states = list(map("".join, combinations))

		result = random.choices(population=states, weights=probs, k=1)[0]  

		return "".join(list(reversed(result)))

H = [1/sqrt(2)+0j, 1/sqrt(2)+0j, 1/sqrt(2)+0j, -1/sqrt(2)+0j]
X = [0j, 1+0j, 1+0j, 0j]
