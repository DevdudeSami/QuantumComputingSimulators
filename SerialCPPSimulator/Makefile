# g++ sim.cpp -o sim -DARMA_DONT_USE_WRAPPER -framework Accelerate -std=c++11
# ./sim

sim: sim.cpp
	gcc sim.cpp -o sim -DARMA_DONT_USE_WRAPPER -framework Accelerate -std=c++11 -Xpreprocessor -fopenmp -lomp sim.cpp