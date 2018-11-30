import numpy as np
import math
import time
# from numba import jit
# from multiprocessing import Pool

# H = np.array([[1,1],[1,-1]], dtype=complex)/math.sqrt(2)
# H = np.random.randn(64,64)
I = np.array([[1,0],[0,1]], dtype=complex)

start_time = time.time()

identity = I.copy()

for i in range(0,12):
	# if(i%10000==0): print(i)
	print(i)
	identity = np.kron(identity,I)
	# H = H/np.linalg.norm(H)



print("--- %s seconds ---" % (time.time() - start_time))