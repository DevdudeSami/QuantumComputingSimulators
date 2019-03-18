//
//  Algorithms.cpp
//  NoMatrixSimulator
//
//  Created by Youssef Moawad on 07/02/2019.
//  Copyright © 2019 Youssef Moawad. All rights reserved.
//

#include "Algorithms.hpp"

#include "Accumulator.hpp"

#include <iostream>

void QuantumFourierTransform(QRegister *reg, vector<QID> qIDs) {
  for(int i = 0; i < qIDs.size(); i++) {
    reg->applySingleGate(qIDs[i], H());
    
    for(int j = i+1; j < qIDs.size(); j++) {
      reg->applyCnGate({qIDs[j]}, qIDs[i], Rm(j-i+1));
    }
  }
}

void InverseQuantumFourierTransform(QRegister *reg, vector<QID> qIDs) {
  for(int i = qIDs.size() - 1; i >= 0; i--) {
    for(int j = qIDs.size() - 1; j > i; j--) {
      reg->applyCnGate({qIDs[j]}, qIDs[i], Rm(-(j-i+1)));
    }
    
    reg->applySingleGate(qIDs[i], H());
  }
}

/*** Grover's Search Algorithm ***/

void GroversOracle(QRegister *reg, vector<uint> markedStates) {
  
  for(auto s: markedStates) {
    reg->applySingleGate(s, X());
  }
  
  // construct vector of controls
  vector<QID> allQs = reg->allQubits();
  vector<QID> controls(allQs.begin(), allQs.end()-1);
  
  reg->applyCnGate(controls, reg->numberOfQubits()-1, X());
  
  for(auto s: markedStates) {
    reg->applySingleGate(s, X());
  }
}

void GroversDiffusion(QRegister *reg) {
  for(uint i = 0; i < reg->numberOfQubits()-1; i++) {
    reg->applySingleGate(i, H());
    reg->applySingleGate(i, X());
  }
  
  // construct vector of controls
  vector<QID> allQs = reg->allQubits();
  vector<QID> controls(allQs.begin(), allQs.end()-2);
  
  reg->applyCnGate(controls, reg->numberOfQubits()-2, Z());
  
  for(uint i = 0; i < reg->numberOfQubits()-1; i++) {
    reg->applySingleGate(i, X());
    reg->applySingleGate(i, H());
  }
  
}

string GroversSearch(uint n, vector<uint> markedStates, uint iters) {
  
  QRegister reg(n+1);
  
  // The last qubit must be flipped
  reg.applySingleGate(n, X());
  
  // H all qubits
  for(uint i = 0; i < n+1; i++) {
    reg.applySingleGate(i, H());
  }
  
  for(int i = 0; i < iters; i++) {
    GroversOracle(&reg, markedStates);
    GroversDiffusion(&reg);
  }
  
  string m = reg.measure();
  
  return m.substr(0,n);
}
