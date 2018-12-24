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

//  cout << QFT(10).toString() << endl;
  
//  QComputer comp (10);

//  QuantumFourierTransform(&comp, comp.allQubits());
//  QFTGate(&comp, comp.allQubits());
//
//  cout << takeMeasurementsInString(comp, 1000, nullptr) << endl;
  
//  Row amps = {1,0,0,0,0,0,0,0};
//  vector<int> qIDs = {0,1,2};
//
//  StateVector state (SparseTensor(Tensor(amps),1), qIDs);
//
//  state.applyNGate(H(), vector<int>({0}));
//  state.applyNGate(CNOT(), vector<int>({0,1}));
//
//  cout << takeMeasurementsInString(state, 1000, nullptr) << endl;

//  const SparseTensor static_i = I();
//  const SparseTensor static_h = H();
//  SparseTensor m = H();

//  for(int i = 0; i < 15; i++) {
//    cout << i << endl;
//    if(i%2 == 0) m = m.kronWith(static_h);
//    else m = m.kronWith(static_i);
//
//    auto end = chrono::high_resolution_clock::now();
//    auto diff = end - start;
//    cout << (chrono::duration <double, milli> (diff).count())/1000 << " s" << endl;
//  }

  // Algorithms testing
  
  // Half Adder testing
//  cout << HalfAdder(0, 0) << endl;
//  cout << HalfAdder(0, 1) << endl;
//  cout << HalfAdder(1, 0) << endl;
//  cout << HalfAdder(1, 1) << endl;
//
//  cout << "-----" << endl;
//
//  // Full Adder testing
//  cout << FullAdder(0, 0, 0) << endl;
//  cout << FullAdder(0, 0, 1) << endl;
//  cout << FullAdder(0, 1, 0) << endl;
//  cout << FullAdder(0, 1, 1) << endl;
//  cout << FullAdder(1, 0, 0) << endl;
//  cout << FullAdder(1, 0, 1) << endl;
//  cout << FullAdder(1, 1, 0) << endl;
//  cout << FullAdder(1, 1, 1) << endl;
  
  // Ripple Carry Adder testing
//  cout << AltSixQubitRippleCarryAdder("111111", "111111") << endl;

  auto end = chrono::high_resolution_clock::now();
  auto diff = end - start;
  cout << "--- end time: " << (chrono::duration <double, milli> (diff).count())/1000 << " s" << endl;

  return 0;
}
