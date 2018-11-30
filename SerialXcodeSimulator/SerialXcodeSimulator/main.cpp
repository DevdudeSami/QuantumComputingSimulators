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
  
  auto start = chrono::steady_clock::now();
  
//  Row amps = {0,0,0,0,1,0,0,0};
//  vector<int> qIDs = {0,1,2};
//
//  StateVector state (Tensor(amps), qIDs);
//
//  state.swap(0, 2);
//
//  cout << state.measure() << endl;

  
  SparseTensor identity (I(), 2);
  
//  cout << identity.kronWith(identity).kronWith(identity).dense().toString() << endl;
  
  for(int i = 0; i < 15; i++) {
    cout << i << endl;
    identity = identity.kronWith(SparseTensor(I(), 2));
  }

  auto end = chrono::steady_clock::now();
  auto diff = end - start;
  cout << (chrono::duration <double, milli> (diff).count())/1000 << " s" << endl;

  return 0;
}
