//
//  Adders.cpp
//  NoMatrixSimulator
//
//  Created by Youssef Moawad on 06/02/2019.
//  Copyright © 2019 Youssef Moawad. All rights reserved.
//

#include "Adders.hpp"


void NBitCuccaroAdderCircuit(uint n, QRegister *reg, vector<QID> qIDs) {
  uint N = 2*n+2;
  
//  assert(reg->numberOfQubits() >= N);
  assert(qIDs.size() == N);
  
  // Step 1
  for(int i = 3; i < N-2; i += 2) {
    reg->applyCnGate({qIDs[i+1]}, qIDs[i], X());
  }
  
  // Step 2
  for(int i = 0; i < N-5; i += 2) {
    reg->applyCnGate({qIDs[i+4]}, qIDs[i+2], X());
    reg->applyCnGate({qIDs[i], qIDs[i+1]}, qIDs[i+2], X());
  }
  
  // Step 3
  reg->applyCnGate({qIDs[N-2]}, qIDs[N-1], X());
  
  // Step 4
  for(int i = 3; i < N-4; i += 2) {
    reg->applySingleGate(qIDs[i], X());
  }
  
  // Step 5
  reg->applyCnGate({qIDs[N-4], qIDs[N-3]}, qIDs[N-1], X());
  
  // Step 6
  for(int i = 2; i < N - 3; i += 2) {
    reg->applyCnGate({qIDs[i]}, qIDs[i+1], X());
  }
  
  // Step 7
  reg->applyCnGate({qIDs[N-6], qIDs[N-5]}, qIDs[N-4], X());
  
  // Step 8
  for(int i = N-8; i >= 0; i -= 2) {
    reg->applyCnGate({qIDs[i], qIDs[i+1]}, qIDs[i+2], X());
    reg->applySingleGate(qIDs[i+3], X());
    reg->applyCnGate({qIDs[i+6]}, qIDs[i+4], X());
  }
  
  // Steps 9 and 10
  reg->applyCnGate({qIDs[4]}, qIDs[2], X());
  reg->applyCnGate({qIDs[1]}, qIDs[0], X());
  
  // Step 11
  for(int i = 3; i < N-2; i += 2) {
    reg->applyCnGate({qIDs[i+1]}, qIDs[i], X());
  }
  
}

string NBitCuccaroAdder(uint n, string A, string B) {
  assert(A.size() == n);
  assert(B.size() == n);
  
  uint N = 2*n+2;
  
  QRegister reg (N);
  
  // Bit 2 is the input carry
  
  // Bits 0,3,5,7,9,11,..,N-3 are B
  // Bits 1,4,6,8,10,12,..,N-2 are A
  if(B[n-1] == '1') reg.applySingleGate(0, X());
  if(A[n-1] == '1') reg.applySingleGate(1, X());
  
  for(int i = 0; i < n-1; i++) {
    if(B[i] == '1') reg.applySingleGate((N-3) - i*2, X());
    if(A[i] == '1') reg.applySingleGate((N-2) - i*2, X());
  }
  
  // The last bit is the Z bit
  
  NBitCuccaroAdderCircuit(n, &reg, reg.allQubits());
  
  string m = reg.measure();
  // Bits N-1,N-3,N-5,N-7,..,3,0 are the sum after the circuit is done
  string result;
  for(int i = N-1; i >= 3; i -= 2) result.push_back(m[i]);
  result.push_back(m[0]);
  
  return result;
}
