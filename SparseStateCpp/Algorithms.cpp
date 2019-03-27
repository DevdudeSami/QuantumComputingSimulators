//
//  Algorithms.cpp
//  MatrixBasedSimulator
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

void QuantumFourierTransform(QComputer *comp, vector<QID> qIDs) {
  for(int i = 0; i < qIDs.size(); i++) {
    HGate(comp, {qIDs[i]});
    
    for(int j = i+1; j < qIDs.size(); j++) {
      CRmGate(j-i+1, comp, {qIDs[j], qIDs[i]});
    }
  }
}


/*** Grover's Search Algorithm ***/

void GroversOracle(QComputer *reg, vector<uint> markedStates) {
  
  SparseTensor CnNOT = CnNOTGate(pow(2, reg->numberOfQubits()));

  for(auto s: markedStates) {
    XGate(reg, {s});
  }
  
  // construct vector of controls
  vector<QID> allQs = reg->allQubits();
  vector<QID> controls(allQs.begin(), allQs.end()-1);
  
  reg->applyMultiGate(reg->allQubits(), CnNOT);
  
  for(auto s: markedStates) {
    XGate(reg, {s});
  }
}

void GroversDiffusion(QComputer *reg) {
  for(uint i = 0; i < reg->numberOfQubits()-1; i++) {
    HGate(reg, {i});
    XGate(reg, {i});
  }
  
  SparseTensor CnZ = CnZGate(pow(2, reg->numberOfQubits()-1));

  vector<QID> allQs = reg->allQubits();
  vector<QID> cnzIDs (allQs.begin(), allQs.end()-1);
  
  reg->applyMultiGate(cnzIDs, CnZ);
  
  for(uint i = 0; i < reg->numberOfQubits()-1; i++) {
    XGate(reg, {i});
    HGate(reg, {i});
  }
  
}

string GroversSearch(uint n, vector<uint> markedStates, uint iters) {
  
  QComputer reg(n+1);
  
  // The last qubit must be flipped
  XGate(&reg, {n});
  
  // H all qubits
  for(uint i = 0; i < n+1; i++) {
    HGate(&reg, {i});
  }
  
  for(int i = 0; i < iters; i++) {
    GroversOracle(&reg, markedStates);
    GroversDiffusion(&reg);
  }
  
  string m = reg.measure();
  
  return m.substr(1,n);
}