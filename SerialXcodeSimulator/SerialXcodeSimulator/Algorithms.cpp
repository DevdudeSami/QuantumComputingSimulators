//
//  Algorithms.cpp
//  SerialXcodeSimulator
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

void CircuitOptimisedQuantumFourierTransform(QComputer *comp, vector<QID> qIDs) {
  CircuitOptimiser co (comp, QuantumFourierTransformGates(qIDs));
  co.executeCircuit();
}

void CircuitOptimisedInverseQuantumFourierTransform(QComputer *comp, vector<QID> qIDs) {
  CircuitOptimiser co (comp, InverseQuantumFourierTransformGates(qIDs));
  co.executeCircuit();
}

/// markedStates is a list of the state indices, i.e. the qubit IDs
vector<ApplicableGate> GroversOracle(QComputer *comp, vector<uint> markedStates) {
  vector<ApplicableGate> gates;
  
  Tensor* X = new SparseTensor(XGate());
  Tensor* CnNOT = new SparseTensor(CnNOTGate(pow(2, comp->numberOfQubits())));
  
  for(auto s: markedStates) {
    gates.push_back(ApplicableGate(X, {s}));
  }
  
  gates.push_back(ApplicableGate(CnNOT, comp->allQubits()));
  
  for(auto s: markedStates) {
    gates.push_back(ApplicableGate(X, {s}));
  }
  
  return gates;
}

vector<ApplicableGate> GroversDiffusion(QComputer *comp) {
  vector<ApplicableGate> gates;
  
  Tensor* H = new SparseTensor(HGate());
  Tensor* X = new SparseTensor(XGate());
  Tensor* CnZ = new SparseTensor(CnZGate(pow(2, comp->numberOfQubits()-1)));
  
  for(uint i = 0; i < comp->numberOfQubits()-1; i++) {
    gates.push_back(ApplicableGate(H, {i}));
    gates.push_back(ApplicableGate(X, {i}));
  }
  
  vector<QID> allQs = comp->allQubits();
  vector<QID> cnzIDs (allQs.begin(), allQs.end()-1);
  
  gates.push_back(ApplicableGate(CnZ, cnzIDs));
  
  for(uint i = 0; i < comp->numberOfQubits()-1; i++) {
    gates.push_back(ApplicableGate(X, {i}));
    gates.push_back(ApplicableGate(H, {i}));
  }
  
  return gates;
}

string GroversSearch(uint n, vector<uint> markedStates, uint iters) {
  vector<ApplicableGate> gates;
  
  QComputer comp(n+1);
  
  // The last qubit must be flipped
  gates.push_back(XAGate({n}));
  
  // H all qubits
  for(uint i = 0; i < n+1; i++) {
    gates.push_back(HAGate({i}));
  }
  
  vector<ApplicableGate> oracle = GroversOracle(&comp, markedStates);
  vector<ApplicableGate> diffusion = GroversDiffusion(&comp);
  
  for(int i = 0; i < iters; i++) {
    gates.insert(gates.end(), oracle.begin(), oracle.end());
    gates.insert(gates.end(), diffusion.begin(), diffusion.end());
  }
  
  CircuitOptimiser co (&comp, gates);
  co.executeCircuit();
  
  string m = comp.measure();
  
  return m.substr(1,n);
}

void Teleport(QComputer *comp, QID source, QID ancillary, QID target) {

  
  // Algorithm from https://qcsimulator.github.io/?example=Quantum%20Teleportation
  
  vector<ApplicableGate> gates;
  
  gates.push_back(HAGate({source}));
  gates.push_back(HAGate({target}));
  
  gates.push_back(CNOTAGate({target, ancillary}));
  
  gates.push_back(CNOTAGate({source, ancillary}));
  
  gates.push_back(HAGate({source}));
  gates.push_back(CNOTAGate({ancillary, target}));
  
  gates.push_back(HAGate({target}));
  
  gates.push_back(CNOTAGate({source, target}));
  
  gates.push_back(HAGate({source}));
  gates.push_back(HAGate({ancillary}));
  
  CircuitOptimiser co (comp, gates);
  co.executeCircuit();
    
  // Entangle target and ancillary
//  HGate(comp, {ancillary});
//  CNOTGate(comp, {ancillary, target});
//
//  // Teleport
//  CNOTGate(comp, {source, ancillary});
//  HGate(comp, {source});

  
  
//  string m = comp->measure();
//  if(m[ancillary+1] == '1') XGate(comp, {target});
//  m = comp->measure();
//  if(m[source+1] == '1') ZGate(comp, {target});
  
}

