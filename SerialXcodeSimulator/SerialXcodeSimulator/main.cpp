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

#include "Tensor.hpp"
#include "Gates.hpp"
#include "StateVector.hpp"
#include "SparseTensor.hpp"
#include "Accumulator.hpp"
#include "QComputer.hpp"
#include "Algorithms.hpp"

using namespace std;

int main(int argc, char const *argv[]) {
  auto start = chrono::high_resolution_clock::now();

//  QComputer comp (6);
  
//  comp.flipQubit(3);
//  comp.flipQubit(2);
//  
//  MAJCircuit(&comp, {2,1,0});
//  MAJCircuit(&comp, {4,3,2});
//  UMACircuit(&comp, {2,3,4});
//  UMACircuit(&comp, {0,1,2});
//
//  comp.applyMultiGate({4,5}, CNOT());
  
//  comp.combineQubits(comp.allQubits());
  
//  comp.applySingleGate(3, H());
//  comp.applySingleGate(1, H());
//  comp.applySingleGate(1, X());
//
//  comp.applyMultiGate({1,5}, CNOT());
  
//  comp.entangleQubits(comp.allQubits());
//  comp.applySingleGate(3, X());
  
//  comp.applySingleGateToMutlipleQubits(vector<QID>({1,5}), X());
//  comp.applyMultiGate(vector<QID>({1,5,3}), TOFF());
  
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
  cout << AltSixQubitRippleCarryAdder("111111", "111111") << endl;

  auto end = chrono::high_resolution_clock::now();
  auto diff = end - start;
  cout << "--- end time: " << (chrono::duration <double, milli> (diff).count())/1000 << " s" << endl;

  return 0;
}
