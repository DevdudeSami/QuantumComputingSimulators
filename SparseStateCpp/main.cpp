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

string NBitCuccaroAdder(uint n, string A, string B);

int main(int argc, char const *argv[]) {
  auto start = chrono::high_resolution_clock::now();

  // Ripple Carry Adder testing
  NBitCuccaroAdder(8, "11111111", "11111111");

  auto end = chrono::high_resolution_clock::now();
  auto diff = end - start;
  cout << (chrono::duration <double, milli> (diff).count())/1000;

  return 0;
}



void NBitCuccaroAdderCircuit(uint n, QComputer *comp, vector<QID> qIDs) {
  uint N = 2*n+2;
  
  assert(comp->numberOfQubits() >= N);
  assert(qIDs.size() == N);
    
  // Step 1
  for(int i = 3; i < N-2; i += 2) {
    CNOTGate(comp, {qIDs[i+1],qIDs[i]});
  }
  
  // Step 2
  for(int i = 0; i < N-5; i += 2) {
    CNOTGate(comp, {qIDs[i+4],qIDs[i+2]});
    TOFFGate(comp, {qIDs[i],qIDs[i+1],qIDs[i+2]});
  }
  
  // Step 3
  CNOTGate(comp, {qIDs[N-2],qIDs[N-1]});
  
  // Step 4
  for(int i = 3; i < N-4; i += 2) {
    XGate(comp, {qIDs[i]});
  }
  
  // Step 5
  TOFFGate(comp, {qIDs[N-4],qIDs[N-3],qIDs[N-1]});
  
  // Step 6
  for(int i = 2; i < N - 3; i += 2) {
    CNOTGate(comp, {qIDs[i],qIDs[i+1]});
  }
  
  // Step 7
  TOFFGate(comp, {qIDs[N-6],qIDs[N-5],qIDs[N-4]});
  
  // Step 8
  for(int i = N-8; i >= 0; i -= 2) {
    TOFFGate(comp, {qIDs[i],qIDs[i+1],qIDs[i+2]});
    XGate(comp, {qIDs[i+3]});
    CNOTGate(comp, {qIDs[i+6],qIDs[i+4]});
  }
  
  // Steps 9 and 10
  CNOTGate(comp, {qIDs[4],qIDs[2]});
  CNOTGate(comp, {qIDs[1],qIDs[0]});
  
  // Step 11
  for(int i = 3; i < N-2; i += 2) {
    CNOTGate(comp, {qIDs[i+1],qIDs[i]});
  }
}


string NBitCuccaroAdder(uint n, string A, string B) {
  assert(A.size() == n);
  assert(B.size() == n);
  
  uint N = 2*n+2;
  
  QComputer comp (N);
  
  // Bit 2 is the input carry
  
  // Bits 0,3,5,7,9,11,..,N-3 are B
  // Bits 1,4,6,8,10,12,..,N-2 are A
  if(B[n-1] == '1') XGate(&comp, {0});
  if(A[n-1] == '1') XGate(&comp, {1});
  
  for(int i = 0; i < n-1; i++) {
    if(B[i] == '1') XGate(&comp, {(N-3) - i*2});
    if(A[i] == '1') XGate(&comp, {(N-2) - i*2});
  }
  
  // The last bit is the Z bit
  
  vector<uint> qubits;
  for(int i = 0; i<N; i++)
    qubits.push_back(i);

  NBitCuccaroAdderCircuit(n, &comp, qubits);
  
  string m = comp.measure();
  // Bits N-1,N-3,N-5,N-7,..,3,0 are the sum after the circuit is done
  string result;
  for(int i = N-1; i >= 3; i -= 2) result.push_back(m[i+1]);
  result.push_back(m[1]);
  
  return result;
}

