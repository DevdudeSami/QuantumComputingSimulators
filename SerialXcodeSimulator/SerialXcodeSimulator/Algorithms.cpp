//
//  Algorithms.cpp
//  SerialXcodeSimulator
//
//  Created by Youssef Moawad on 09/12/2018.
//  Copyright © 2018 Youssef Moawad. All rights reserved.
//

#include "Algorithms.hpp"


void HalfAdderCircuit(QComputer *comp, vector<QID> qIDs) {
  assert(comp->numberOfQubits() >= 3);
  assert(qIDs.size() == 3);
  
  TOFFGate(comp, qIDs);
  CNOTGate(comp, {qIDs[0], qIDs[1]});
}

string HalfAdder(uint A, uint B) {
  QComputer comp (3);
  
  if(A == 1) comp.flipQubit(0);
  if(B == 1) comp.flipQubit(1);
  
  HalfAdderCircuit(&comp, {0,1,2});
  
  string m = comp.measure();
  
  return {m[2], m[3]};
}

void FullAdderCircuit(QComputer *comp, vector<QID> qIDs) {
  assert(comp->numberOfQubits() >= 4);
  assert(qIDs.size() == 4);
  
  HalfAdderCircuit(comp, {qIDs[0], qIDs[1], qIDs[3]});
  HalfAdderCircuit(comp, {qIDs[1], qIDs[2], qIDs[3]});
  CNOTGate(comp, {qIDs[0], qIDs[1]});
}

string FullAdder(uint A, uint B, uint C) {
  QComputer comp (4);
  
  if(A == 1) comp.flipQubit(0);
  if(B == 1) comp.flipQubit(1);
  if(C == 1) comp.flipQubit(2);
  
  FullAdderCircuit(&comp, {0,1,2,3});
  
  string m = comp.measure();
  
  return {m[4], m[3]};
}

void MAJCircuit(QComputer *comp, vector<QID> qIDs) {
  assert(comp->numberOfQubits() >= 3);
  assert(qIDs.size() == 3);
  
  CNOTGate(comp, {qIDs[2], qIDs[1]});
  CNOTGate(comp, {qIDs[2], qIDs[0]});
  TOFFGate(comp, qIDs);
}

void UMACircuit(QComputer *comp, vector<QID> qIDs) {
  assert(comp->numberOfQubits() >= 3);
  assert(qIDs.size() == 3);
  
  TOFFGate(comp, qIDs);
  CNOTGate(comp, {qIDs[2], qIDs[0]});
  CNOTGate(comp, {qIDs[0], qIDs[1]});

//  comp->flipQubit(qIDs[1]);
//  comp->applyMultiGate({qIDs[0], qIDs[1]}, CNOT());
//  comp->applyMultiGate(qIDs, TOFF());
//  comp->flipQubit(qIDs[1]);
//  comp->applyMultiGate({qIDs[2], qIDs[0]}, CNOT());
//  comp->applyMultiGate({qIDs[2], qIDs[1]}, CNOT());
}

void SixQubitRippleCarryAdderCircuit(QComputer *comp, vector<QID> qIDs) {
  
  assert(comp->numberOfQubits() >= 14);
  assert(qIDs.size() == 14);
  
  MAJCircuit(comp, {qIDs[0], qIDs[1], qIDs[2]});
  MAJCircuit(comp, {qIDs[2], qIDs[3], qIDs[4]});
  MAJCircuit(comp, {qIDs[4], qIDs[5], qIDs[6]});
  MAJCircuit(comp, {qIDs[6], qIDs[7], qIDs[8]});
  MAJCircuit(comp, {qIDs[8], qIDs[9], qIDs[10]});
  MAJCircuit(comp, {qIDs[10], qIDs[11], qIDs[12]});
  
  CNOTGate(comp, {qIDs[12], qIDs[13]});
  
  UMACircuit(comp, {qIDs[10], qIDs[11], qIDs[12]});
  UMACircuit(comp, {qIDs[8], qIDs[9], qIDs[10]});
  UMACircuit(comp, {qIDs[6], qIDs[7], qIDs[8]});
  UMACircuit(comp, {qIDs[4], qIDs[5], qIDs[6]});
  UMACircuit(comp, {qIDs[2], qIDs[3], qIDs[4]});
  UMACircuit(comp, {qIDs[0], qIDs[1], qIDs[2]});
}

string SixQubitRippleCarryAdder(string A, string B) {
  QComputer comp (14);
  
  // Bit 0 is the input carry
  
  // Bits 1,3,5,7,9,11 are B
  if(B[5] == '1') comp.flipQubit(1);
  if(B[4] == '1') comp.flipQubit(3);
  if(B[3] == '1') comp.flipQubit(5);
  if(B[2] == '1') comp.flipQubit(7);
  if(B[1] == '1') comp.flipQubit(9);
  if(B[0] == '1') comp.flipQubit(11);
  
  // Bits 2,4,6,8,10,12 are A
  if(A[5] == '1') comp.flipQubit(2);
  if(A[4] == '1') comp.flipQubit(4);
  if(A[3] == '1') comp.flipQubit(6);
  if(A[2] == '1') comp.flipQubit(8);
  if(A[1] == '1') comp.flipQubit(10);
  if(A[0] == '1') comp.flipQubit(12);
  
  // Bit 13 is the Z bit
  
  SixQubitRippleCarryAdderCircuit(&comp, comp.allQubits());
  
  string m = comp.measure();
  cout << m << endl;
  // Bits 1,3,5,7,9,11 are the sum after the circuit is done
  return {m[14], m[12], m[10], m[8], m[6], m[4], m[2]};
}

/// This circuit actually properly works; however creating a general case of it will be significantly harder. However, this has greater parallelism potential.
void AltSixQubitRippleCarryAdderCircuit(QComputer *comp, vector<QID> qIDs) {
  
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
  comp->flipQubit(3);
  comp->flipQubit(5);
  comp->flipQubit(7);
  comp->flipQubit(9);
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
  comp->flipQubit(9);
  CNOTGate(comp, {qIDs[12], qIDs[10]});
  
  // Time slice 12
  TOFFGate(comp, {qIDs[4], qIDs[5], qIDs[6]});
  comp->flipQubit(7);
  CNOTGate(comp, {qIDs[10], qIDs[8]});
  
  // Time slice 13
  TOFFGate(comp, {qIDs[2], qIDs[3], qIDs[4]});
  comp->flipQubit(5);
  CNOTGate(comp, {qIDs[8], qIDs[6]});
  
  // Time slice 14
  TOFFGate(comp, {qIDs[0], qIDs[1], qIDs[2]});
  comp->flipQubit(3);
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

string AltSixQubitRippleCarryAdder(string A, string B) {
  QComputer comp (14);
  
  // Bit 2 is the input carry
  
  // Bits 0,3,5,7,9,11 are B
  if(B[5] == '1') comp.flipQubit(0);
  if(B[4] == '1') comp.flipQubit(3);
  if(B[3] == '1') comp.flipQubit(5);
  if(B[2] == '1') comp.flipQubit(7);
  if(B[1] == '1') comp.flipQubit(9);
  if(B[0] == '1') comp.flipQubit(11);
  
  // Bits 1,4,6,8,10,12 are A
  if(A[5] == '1') comp.flipQubit(1);
  if(A[4] == '1') comp.flipQubit(4);
  if(A[3] == '1') comp.flipQubit(6);
  if(A[2] == '1') comp.flipQubit(8);
  if(A[1] == '1') comp.flipQubit(10);
  if(A[0] == '1') comp.flipQubit(12);
  
  // Bit 13 is the Z bit
  
  AltSixQubitRippleCarryAdderCircuit(&comp, comp.allQubits());
  
  string m = comp.measure();
  // Bits 0,3,5,7,9,11 are the sum after the circuit is done
  return {m[14], m[12], m[10], m[8], m[6], m[4], m[1]};
}

// TODO: Fix this; the qubit IDs should be indices of the passed qIDs vector
void CircuitOptimisedAltSixQubitRippleCarryAdderCircuit(QComputer *comp, vector<QID> qIDs) {
  
  assert(comp->numberOfQubits() >= 14);
  assert(qIDs.size() == 14);
  
  vector<ApplicableGate> gates = {
    // Time slice 1
    CNOTAGate({4,3}),
    CNOTAGate({6,5}),
    CNOTAGate({8,7}),
    CNOTAGate({10,9}),
    CNOTAGate({12,11}),
    
    // Time slice 2
    CNOTAGate({4,2}),
    
    // Time slice 3
    TOFFAGate({0,1,2}),
    CNOTAGate({6,4}),
    
    // Time slice 4
    TOFFAGate({2,3,4}),
    CNOTAGate({8,6}),
    
    // Time slice 5
    TOFFAGate({4,5,6}),
    CNOTAGate({10,8}),

    // Time slice 6
    TOFFAGate({6,7,8}),
    CNOTAGate({12,10}),

    // Time slice 7
    TOFFAGate({8,9,10}),
    CNOTAGate({12,13}),
    
    // Time slice 8
    XAGate({3}),
    XAGate({5}),
    XAGate({7}),
    XAGate({9}),
    TOFFAGate({10,11,13}),
    
    // Time slice 9
    CNOTAGate({2,3}),
    CNOTAGate({4,5}),
    CNOTAGate({6,7}),
    CNOTAGate({8,9}),
    CNOTAGate({10,11}),
    
    // Time slice 10
    TOFFAGate({8,9,10}),
    
    // Time slice 11
    TOFFAGate({6,7,8}),
    XAGate({9}),
    CNOTAGate({12,10}),
    
    // Time slice 12
    TOFFAGate({4,5,6}),
    XAGate({7}),
    CNOTAGate({10,8}),
    
    // Time slice 13
    TOFFAGate({2,3,4}),
    XAGate({5}),
    CNOTAGate({8,6}),
    
    // Time slice 14
    TOFFAGate({0,1,2}),
    XAGate({3}),
    CNOTAGate({6,4}),
    
    // Time slice 15
    CNOTAGate({4,2}),
    
    // Time slice 16
    CNOTAGate({1,0}),
    CNOTAGate({4,3}),
    CNOTAGate({6,5}),
    CNOTAGate({8,7}),
    CNOTAGate({10,9}),
    CNOTAGate({12,11}),
  };
  
  CircuitOptimiser co (comp, gates);
  co.executeCircuit();
}

string CircuitOptimisedAltSixQubitRippleCarryAdder(string A, string B) {
  QComputer comp (14);
  
  // Bit 2 is the input carry
  
  // Bits 0,3,5,7,9,11 are B
  if(B[5] == '1') comp.flipQubit(0);
  if(B[4] == '1') comp.flipQubit(3);
  if(B[3] == '1') comp.flipQubit(5);
  if(B[2] == '1') comp.flipQubit(7);
  if(B[1] == '1') comp.flipQubit(9);
  if(B[0] == '1') comp.flipQubit(11);
  
  // Bits 1,4,6,8,10,12 are A
  if(A[5] == '1') comp.flipQubit(1);
  if(A[4] == '1') comp.flipQubit(4);
  if(A[3] == '1') comp.flipQubit(6);
  if(A[2] == '1') comp.flipQubit(8);
  if(A[1] == '1') comp.flipQubit(10);
  if(A[0] == '1') comp.flipQubit(12);
  
  // Bit 13 is the Z bit
  
  CircuitOptimisedAltSixQubitRippleCarryAdderCircuit(&comp, comp.allQubits());
  
  string m = comp.measure();
  // Bits 0,3,5,7,9,11 are the sum after the circuit is done
  return {m[14], m[12], m[10], m[8], m[6], m[4], m[1]};
}

void QuantumFourierTransform(QComputer *comp, vector<QID> qIDs) {
  for(int i = 0; i < qIDs.size(); i++) {
    HGate(comp, {qIDs[i]});
    
    for(int j = i+1; j < qIDs.size(); j++) {
      CRmGate(j-i+1, comp, {qIDs[j], qIDs[i]});
    }
  }
}

void CircuitOptimisedQuantumFourierTransform(QComputer *comp, vector<QID> qIDs) {
  vector<ApplicableGate> gates;
  
  for(int i = 0; i < qIDs.size(); i++) {
    gates.push_back(HAGate({qIDs[i]}));
    
    for(int j = i+1; j < qIDs.size(); j++) {
      SparseTensor crmGate = CRm(j-i+1);
      
      gates.push_back(ApplicableGate(&crmGate, {qIDs[j], qIDs[i]}));
    }
  }
  
  CircuitOptimiser co (comp, gates);
  co.executeCircuit();
}

void CircuitOptimisedTwoQubitEntanglement(QComputer *comp, vector<QID> qIDs) {
  assert(comp->numberOfQubits() >= 2);
  assert(qIDs.size() == 2);
  
  vector<ApplicableGate> gates = {
    HAGate({0}),
    CNOTAGate({0,1})
  };
  
  CircuitOptimiser co (comp, gates);
  co.executeCircuit();
}


void CircuitOptimisedNBitCuccaroAdderCircuit(uint n, QComputer *comp, vector<QID> qIDs) {
  uint N = 2*n+2;
  
  assert(comp->numberOfQubits() >= N);
  assert(qIDs.size() == N);
  
  
  vector<ApplicableGate> gates;
  
  // Step 1
  for(int i = 3; i < N-2; i += 2) {
    gates.push_back(CNOTAGate({qIDs[i+1],qIDs[i]}));
  }
  
  // Step 2
  for(int i = 0; i < N-5; i += 2) {
    gates.push_back(CNOTAGate({qIDs[i+4],qIDs[i+2]}));
    gates.push_back(TOFFAGate({qIDs[i],qIDs[i+1],qIDs[i+2]}));
  }
  
  // Step 3
  gates.push_back(CNOTAGate({qIDs[N-2],qIDs[N-1]}));
  
  // Step 4
  for(int i = 3; i < N-4; i += 2) {
    gates.push_back(XAGate({qIDs[i]}));
  }
  
  // Step 5
  gates.push_back(TOFFAGate({qIDs[N-4],qIDs[N-3],qIDs[N-1]}));
  
  // Step 6
  for(int i = 2; i < N - 3; i += 2) {
    gates.push_back(CNOTAGate({qIDs[i],qIDs[i+1]}));
  }
  
  // Step 7
  gates.push_back(TOFFAGate({qIDs[N-6],qIDs[N-5],qIDs[N-4]}));
  
  // Step 8
  for(int i = N-8; i >= 0; i -= 2) {
    gates.push_back(TOFFAGate({qIDs[i],qIDs[i+1],qIDs[i+2]}));
    gates.push_back(XAGate({qIDs[i+3]}));
    gates.push_back(CNOTAGate({qIDs[i+6],qIDs[i+4]}));
  }
  
  // Steps 9 and 10
  gates.push_back(CNOTAGate({qIDs[4],qIDs[2]}));
  gates.push_back(CNOTAGate({qIDs[1],qIDs[0]}));
  
  // Step 11
  for(int i = 3; i < N-2; i += 2) {
    gates.push_back(CNOTAGate({qIDs[i+1],qIDs[i]}));
  }
  
  CircuitOptimiser co (comp, gates);
  co.executeCircuit();
}

string CircuitOptimisedNQubitCuccaroAdder(uint n, string A, string B) {
  assert(A.size() == n);
  assert(B.size() == n);
  
  uint N = 2*n+2;
  
  QComputer comp (N);
  
  // Bit 2 is the input carry
  
  // Bits 0,3,5,7,9,11,..,N-3 are B
  // Bits 1,4,6,8,10,12,..,N-2 are A
  if(B[n-1] == '1') comp.flipQubit(0);
  if(A[n-1] == '1') comp.flipQubit(1);
  
  for(int i = 0; i < n-1; i++) {
    if(B[i] == '1') comp.flipQubit((N-3) - i*2);
    if(A[i] == '1') comp.flipQubit((N-2) - i*2);
  }
  
  // The last bit is the Z bit
  
  CircuitOptimisedNBitCuccaroAdderCircuit(n, &comp, comp.allQubits());
  
  string m = comp.measure();
  // Bits N-1,N-3,N-5,N-7,..,3,0 are the sum after the circuit is done
  string result;
  for(int i = N-1; i >= 3; i -= 2) result.push_back(m[i+1]);
  result.push_back(m[1]);
  
  return result;
}

