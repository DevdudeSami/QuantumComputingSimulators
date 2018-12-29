//
//  main.cpp
//  SerialXcodeSimulator
//
//  Created by Youssef Moawad on 02/11/2018.
//  Copyright © 2018 Youssef Moawad. All rights reserved.
//

#include <iostream>
#include <time.h>
//#include <omp.h>
#include <mpi.h>

#include "DenseTensor.hpp"
#include "Gates.hpp"
#include "StateVector.hpp"
#include "SparseTensor.hpp"
#include "Accumulator.hpp"
#include "QComputer.hpp"
#include "Algorithms.hpp"

using namespace std;

int main(int argc, char const *argv[]) {
  auto start = chrono::high_resolution_clock::now();
  
//  QComputer comp (13);
//  QuantumFourierTransform(&comp, comp.allQubits());
//
//  CircuitOptimisedQuantumFourierTransform(&comp, comp.allQubits());
//  cout << takeMeasurementsInString(comp, 1000, nullptr) << endl;
  
//  cout << CircuitOptimisedAltSixQubitRippleCarryAdder("100101", "001011") << endl;
//  cout << AltSixQubitRippleCarryAdder("100101", "001011") << endl;
  
  cout << CircuitOptimisedNQubitCuccaroAdder(8, "11100001", "11100001") << endl;
  
  auto end = chrono::high_resolution_clock::now();
  auto diff = end - start;
  cout << "--- end time: " << (chrono::duration <double, milli> (diff).count())/1000 << " s" << endl;

  return 0;
}
