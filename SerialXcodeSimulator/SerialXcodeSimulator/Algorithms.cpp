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

