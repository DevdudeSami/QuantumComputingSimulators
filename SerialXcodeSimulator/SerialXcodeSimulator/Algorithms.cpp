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
  
  comp->applyMultiGate(qIDs, TOFF());
  comp->applyMultiGate({qIDs[0], qIDs[1]}, CNOT());
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
  comp->applyMultiGate({qIDs[0], qIDs[1]}, CNOT());
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





