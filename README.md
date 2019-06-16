# Development and Optimisation of Quantum Computing Simulators with a Study of Quantum Algorithms

My Joint Honours BSc Level 4 Computing Science Individual Project on Quantum Computing Simulators. 

## What is provided?

- MBS: The CircuitOptimiserCpp matrix-based simulator with QLI available.
- MFS: The final matrix-free simulator.

## Running instructions

Ensure gnu++14 is available and run `make run` in either of the simulators. For anything above the BaseCpp (requiring OpenMP acceleration), OpenMP must be available. The `main()` functions in any of the C++ ones can be changed to run some algorithm or circuit. *The makefile may need to be changed to appropriately link OpenMP depending on the OS. This makefile was last tested on a laptop running macOS 10.14.*

To run a QLI program, enter it in the main.qli file in the QLITest folder. Then go into the `main()` function of MBSPlusQLI and enter the **absolute path** to the QLITest folder on your machine. Finally run `make run`.

