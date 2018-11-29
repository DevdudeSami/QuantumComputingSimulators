import numpy as np
import scipy.sparse as scsp
import math
import time
# from numba import jit
# from multiprocessing import Pool

# H = np.array([[1,1],[1,-1]], dtype=complex)/math.sqrt(2)
# H = np.random.randn(64,64)
I = np.array([[1,0],[0,1]], dtype=complex)

start_time = time.time()

identity = scsp.csr_matrix(I.copy())

for i in range(0,13):
	# if(i%10000==0): print(i)
	print(i)
	identity = scsp.kron(identity,I)
	# H = H/np.linalg.norm(H)



print("--- %s seconds ---" % (time.time() - start_time))