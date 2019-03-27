//
//  main.cpp
//  MatrixBasedSimulator
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

using namespace std;

string SixQubitRippleCarryAdder(string A, string B);
string NBitCuccaroAdder(uint n, string A, string B);

int main(int argc, char const *argv[]) {
  
  auto start = chrono::high_resolution_clock::now();
  
  auto end = chrono::high_resolution_clock::now();
  auto diff = end - start;
  cout << (chrono::duration <double, milli> (diff).count())/1000 << " s" << endl;

  return 0;
}


/*** Algorithm Implementations ***/

void SixQubitRippleCarryAdderCircuit(StateVector *comp, vector<QID> qIDs) {
  
  assert(comp->numberOfQubits() >= 14);
  assert(qIDs.size() == 14);
  
  // Time slice 1
  CNOTGate(comp, {qIDs[4], qIDs[3]});
  CNOTGate(comp, {qIDs[6], qIDs[5]});
  CNOTGate(comp, {qIDs[8], qIDs[7]});
  CNOTGate(comp, {qIDs[10], qIDs[9]});
  CNOTGate(comp, {qIDs[12], qIDs[11]});
  
  // Time slice 2
  CNOTGate(comp, {qIDs[4], qIDs[2]});
  
  // Time slice 3
  TOFFGate(comp, {qIDs[0], qIDs[1], qIDs[2]});
  CNOTGate(comp, {qIDs[6], qIDs[4]});
  
  // Time slice 4
  TOFFGate(comp, {qIDs[2], qIDs[3], qIDs[4]});
  CNOTGate(comp, {qIDs[8], qIDs[6]});
  
  // Time slice 5
  TOFFGate(comp, {qIDs[4], qIDs[5], qIDs[6]});
  CNOTGate(comp, {qIDs[10], qIDs[8]});
  
  // Time slice 6
  TOFFGate(comp, {qIDs[6], qIDs[7], qIDs[8]});
  CNOTGate(comp, {qIDs[12], qIDs[10]});
  
  // Time slice 7
  TOFFGate(comp, {qIDs[8], qIDs[9], qIDs[10]});
  CNOTGate(comp, {qIDs[12], qIDs[13]});
  
  // Time slice 8
  XGate(comp, {3});
  XGate(comp, {5});
  XGate(comp, {7});
  XGate(comp, {9});
  TOFFGate(comp, {qIDs[10], qIDs[11], qIDs[13]});
  
  // Time slice 9
  CNOTGate(comp, {qIDs[2], qIDs[3]});
  CNOTGate(comp, {qIDs[4], qIDs[5]});
  CNOTGate(comp, {qIDs[6], qIDs[7]});
  CNOTGate(comp, {qIDs[8], qIDs[9]});
  CNOTGate(comp, {qIDs[10], qIDs[11]});
  
  // Time slice 10
  TOFFGate(comp, {qIDs[8], qIDs[9], qIDs[10]});
  
  // Time slice 11
  TOFFGate(comp, {qIDs[6], qIDs[7], qIDs[8]});
  XGate(comp, {9});
  CNOTGate(comp, {qIDs[12], qIDs[10]});
  
  // Time slice 12
  TOFFGate(comp, {qIDs[4], qIDs[5], qIDs[6]});
  XGate(comp, {7});
  CNOTGate(comp, {qIDs[10], qIDs[8]});
  
  // Time slice 13
  TOFFGate(comp, {qIDs[2], qIDs[3], qIDs[4]});
  XGate(comp, {5});
  CNOTGate(comp, {qIDs[8], qIDs[6]});
  
  // Time slice 14
  TOFFGate(comp, {qIDs[0], qIDs[1], qIDs[2]});
  XGate(comp, {3});
  CNOTGate(comp, {qIDs[6], qIDs[4]});
  
  // Time slice 15
  CNOTGate(comp, {qIDs[4], qIDs[2]});
  
  // Time slice 16
  CNOTGate(comp, {qIDs[1], qIDs[0]});
  CNOTGate(comp, {qIDs[4], qIDs[3]});
  CNOTGate(comp, {qIDs[6], qIDs[5]});
  CNOTGate(comp, {qIDs[8], qIDs[7]});
  CNOTGate(comp, {qIDs[10], qIDs[9]});
  CNOTGate(comp, {qIDs[12], qIDs[11]});
}

string SixQubitRippleCarryAdder(string A, string B) {
  StateVector comp (14);
  
  // Bit 2 is the input carry
  
  // Bits 0,3,5,7,9,11 are B
  if(B[5] == '1') XGate(&comp, {0});
  if(B[4] == '1') XGate(&comp, {3});
  if(B[3] == '1') XGate(&comp, {5});
  if(B[2] == '1') XGate(&comp, {7});
  if(B[1] == '1') XGate(&comp, {9});
  if(B[0] == '1') XGate(&comp, {11});
  
  // Bits 1,4,6,8,10,12 are A
  if(A[5] == '1') XGate(&comp, {1});
  if(A[4] == '1') XGate(&comp, {2});
  if(A[3] == '1') XGate(&comp, {4});
  if(A[2] == '1') XGate(&comp, {6});
  if(A[1] == '1') XGate(&comp, {8});
  if(A[0] == '1') XGate(&comp, {10});
  
  // Bit 13 is the Z bit
  
  SixQubitRippleCarryAdderCircuit(&comp, {0,1,2,3,4,5,6,7,8,9,10,11,12,13});
  
  string m = comp.measure();
  // Bits 0,3,5,7,9,11 are the sum after the circuit is done
  return {m[14], m[12], m[10], m[8], m[6], m[4], m[1]};
}


















void NBitCuccaroAdderCircuit(uint n, StateVector *comp, vector<QID> qIDs) {
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
  
  StateVector comp (N);
  
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

