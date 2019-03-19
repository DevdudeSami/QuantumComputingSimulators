import cmath
import math
import numpy as np

class SMatrix:
	def __init__(self, d, r, c):
		self.r = r
		self.c = c
		self.d = d

	@staticmethod
	def fromDense(rows):
		assert(len(rows) > 0)
		assert(len(rows[0]) > 0)

		r = len(rows)
		c = len(rows[0])
		d = {}

		for i in range(r):
			for j in range(c):
				if rows[i][j] != 0: d[(i,j)] = rows[i][j]

		return SMatrix(d,r,c)

	@staticmethod
	def onesAt(positions, r, c):
		d = {}
		for p in positions: d[p] = 1
		
		return SMatrix(d,r,c)

	def addTo(self, m):
		assert(self.r == m.r)
		assert(self.c == m.c)

		d_final = {}

		k1 = self.d.keys()
		k2 = m.d.keys()

		for k in k1:
			if k in k2: d_final[k] = self.d[k] + m.d[k]
			else: d_final[k] = self.d[k]
		for k in k2:
			if k not in k1: d_final[k] = m.d[k]
		
		return SMatrix(d_final,self.r,self.c)

	def kMultiplyTo(self, k):
		d_final = {}

		for key in self.d.keys():
			d_final[key] = k*self.d[key]

		return SMatrix(d_final,self.r,self.c)

	def multiplyTo(self, m):
		d_final = {}

		keys_1 = self.d.keys()
		keys_2 = m.d.keys()

		for k1 in keys_1:
			for k2 in keys_2:
				if k1[1] == k2[0]:
					if (k1[0], k2[1]) in d_final.keys(): d_final[(k1[0], k2[1])] += self.d[k1]*m.d[k2]
					else: d_final[(k1[0], k2[1])] = self.d[k1]*m.d[k2]

		return SMatrix(d_final,self.r,m.c)

	def kronWith(self, m):
		d_final = {}

		keys_1 = self.d.keys()
		keys_2 = m.d.keys()

		for k1 in keys_1:
			for k2 in keys_2:
				d_final[(k1[0]*m.r+k2[0],k1[1]*m.c+k2[1])] = self.d[k1] * m.d[k2]

		return SMatrix(d_final, self.r*m.r, self.c*m.c)


	@property
	def dense(self):
		result = np.zeros((self.r, self.c), dtype=complex)
		
		for i in range(self.r):
			for j in range(self.c):
				if (i,j) in self.d.keys(): result[i][j] = self.d[(i,j)]
				else: result[i][j] = 0

		return result


M = SMatrix.fromDense([[4,5,6,7],[1,2,3,4]])
M2 = SMatrix.fromDense([[6,7],[89,9],[3,4],[5,6]])
I = SMatrix.onesAt([(0,0),(1,1)],2,2)

H = SMatrix.fromDense([[1,1],[1,-1]]).kMultiplyTo(1/math.sqrt(2))


# M2 = M.multiplyTo(M2)
# print(M2.dense)

# H = I.kronWith(H).kronWith(H)
# print(H.dense)
	
		
m = SMatrix.fromDense([[1,1],[1,-1]]).kMultiplyTo(1/math.sqrt(2))
m = m.kronWith(m).kMultiplyTo(4)
m = m.multiplyTo(m)

print(m.dense)





	