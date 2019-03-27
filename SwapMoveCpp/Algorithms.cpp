//
//  Algorithms.cpp
//  MatrixBasedSimulator
//
//  Created by Youssef Moawad on 09/12/2018.
//  Copyright © 2018 Youssef Moawad. All rights reserved.
//

#include "Algorithms.hpp"

void QuantumFourierTransform(QComputer *comp, vector<QID> qIDs) {
  for(int i = 0; i < qIDs.size(); i++) {
    HGate(comp, {qIDs[i]});
    
    for(int j = i+1; j < qIDs.size(); j++) {
      CRmGate(j-i+1, comp, {qIDs[j], qIDs[i]});
    }
  }
}

void InverseQuantumFourierTransform(QComputer *comp, vector<QID> qIDs) { 
  for(int i = qIDs.size() - 1; i >= 0; i--) {
    for(int j = qIDs.size() - 1; j > i; j--) {
      InverseCRmGate(j-i+1, comp, {qIDs[j], qIDs[i]});
    }
    
    HGate(comp, {qIDs[i]});
  }
}

vector<ApplicableGate> QuantumFourierTransformGates(vector<QID> qIDs) {
  vector<ApplicableGate> gates;
  
  for(int i = 0; i < qIDs.size(); i++) {
    gates.push_back(HAGate({qIDs[i]}));
    
    for(int j = i+1; j < qIDs.size(); j++) {
      Tensor* crmGate = new SparseTensor(CRm(j-i+1));
      
      gates.push_back(ApplicableGate(crmGate, {qIDs[j], qIDs[i]}));
    }
  }
  
  return gates;
}

vector<ApplicableGate> InverseQuantumFourierTransformGates(vector<QID> qIDs) {
  vector<ApplicableGate> gates;
  
  for(int i = qIDs.size() - 1; i >= 0; i--) {
    for(int j = qIDs.size() - 1; j > i; j--) {
      Tensor* crmGate = new SparseTensor(InverseCRm((j-i+1)));
      
      gates.push_back(ApplicableGate(crmGate, {qIDs[j], qIDs[i]}));
    }
    
    gates.push_back(HAGate({qIDs[i]}));
  }
  
  return gates;
}


/*** Grover's Search Algorithm ***/

void GroversOracle(QComputer *reg, vector<uint> markedStates) {
  
  Tensor* CnNOT = new SparseTensor(CnNOTGate(pow(2, reg->numberOfQubits())));

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
  
  Tensor* CnZ = new SparseTensor(CnZGate(pow(2, reg->numberOfQubits()-1)));

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


