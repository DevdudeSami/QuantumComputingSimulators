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

using namespace std;

int main(int argc, char const *argv[]) {
  auto start = chrono::high_resolution_clock::now();

  QComputer comp (5);
  
  comp.applySingleGate(0, H());
//  comp.applySingleGate(0, X());

  comp.applyMultiGate(vector<QID>({0,1}), CNOT());
  
  cout << comp.measure() << endl;
  
//  Row amps = {1,0,0,0,0,0,0,0};
//  vector<int> qIDs = {0,1,2};
//
//  StateVector state (SparseTensor(Tensor(amps),1), qIDs);
//
//  state.applyNGate(H(), vector<int>({0}));
//  state.applyNGate(CNOT(), vector<int>({0,1}));
//
//  cout << takeMeasurementsInString(state, 1000, nullptr) << endl;

//  const SparseTensor static_m (H(), 4);
//  SparseTensor m = H();
//
//  SparseTensor m1 = m.kronWith(m).multiplyTo(4);
//  SparseTensor m2 = m1.multiplyTo(m1);
//
//  cout << m2.toString() << endl;

//  cout << m.kronWith(m).multiplyTo(4).dense().toString() << endl;

//  for(int i = 0; i < 10; i++) {
//    cout << i << endl;
//    m = m.kronWith(static_m);
//    m = m.multiplyTo(4);
//    m.multiplyTo(m);
//  }
  
  
  // Transpose test
//  SparseTensor swap (SWAP(),6);
//  cout << swap.transpose().dense().toString() << endl;

  auto end = chrono::high_resolution_clock::now();
  auto diff = end - start;
  cout << (chrono::duration <double, milli> (diff).count())/1000 << " s" << endl;

  return 0;
}
