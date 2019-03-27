# Development and Optimisation of Quantum Computing Simulators with a Study of Quantum Algorithms

My Joint Honours BSc Level 4 Computing Science Individual Project on Quantum Computing Simulators. 

## What is provided?

Provided are folders containing the simulators at different levels of optimisation. Particularly:

- BasePython: The first prototype of the matrix-based simulator, in Python and NumPy.
- BaseCpp: A direct port of the BasePython simulator to C++, with a custom matrix implementation.
- ParallelisedBaseCpp: BaseCpp with OpenMP acceleration for matrix operations.
- SparseStateCpp: The MBS with the memory optimisations: sparse matrices and the state vector "on-demand" optimisation.
- TensorProductCpp: The MBS with the memory optimisations plus the tensor product with identity matrix optimisation.
- SwapMoveCpp: The MBS at TensorProductCpp plus the improved swap/move optimisation.
- CircuitOptimiserCpp: The MBS at SwapMoveCpp plus the CircuitOptimiser class for parallelising gate preparation.
- MatrixFreePython: A prototype of the matrix-free simulator in Python.
- MatrixFreeCpp: The final matrix-free simulator, in C++.
- MBSPlusQLI: The CircuitOptimiserCpp MBS with QLI available.

## Running instructions

To run any of the C++ ones, ensure gnu++14 is available and run `make run`. For anything above the BaseCpp (requiring OpenMP acceleration), OpenMP must be available. The `main()` functions in any of the C++ ones can be changed to run some algorithm or circuit. *The makefile may need to be changed to appropriately link OpenMP depending on the OS. This makefile was last tested on a macOS 10.14.*

To run a QLI program, enter it in the main.qli file in the QLITest folder. Then go into the `main()` function of MBSPlusQLI and enter the **absolute path** to the QLITest folder on your machine. Finally run `make run`.

