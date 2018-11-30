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
#include "ArrayTensor.hpp"
#include "Gates.hpp"
#include "StateVector.hpp"
#include "SparseTensor.hpp"

using namespace std;

int main(int argc, char const *argv[]) {
  
  auto start = chrono::high_resolution_clock::now();
  
//  Row amps = {0,0,0,0,1,0,0,0};
//  vector<int> qIDs = {0,1,2};
//
//  StateVector state (Tensor(amps), qIDs);
//
//  state.swap(0, 2);
//
//  cout << state.measure() << endl;

  SparseTensor identity (H(), 4);
  
//  Tensor identity = I();
  
//  cout << identity.kronWith(identity).kMultiplyTo(2).dense().toString() << endl;
  
  for(int i = 0; i < 12; i++) {
    cout << i << endl;
    identity = identity.kronWith(SparseTensor(H(), 4));
    identity = identity.kMultiplyTo(4);
  }

  auto end = chrono::high_resolution_clock::now();
  auto diff = end - start;
  cout << (chrono::duration <double, milli> (diff).count())/1000 << " s" << endl;

  return 0;
}
