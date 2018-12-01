import numpy as np
import scipy.sparse as scsp
import math
import time
# from numba import jit
# from multiprocessing import Pool

H = scsp.dok_matrix(np.array([[1,1],[1,-1]], dtype=complex)/math.sqrt(2))
I = scsp.dok_matrix(np.array([[1,0],[0,1]], dtype=complex))

m = H.copy()
# identity = I.copy()

start_time = time.time()

for i in range(0,13):
	print(i)
	m = scsp.kron(m,H)
	m = m * 4
	# identity = np.kron(identity,I)



print("--- %s seconds ---" % (time.time() - start_time))