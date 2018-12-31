//
//  CircuitOptimiser.hpp
//  SerialXcodeSimulator
//
//  Created by Youssef Moawad on 25/12/2018.
//  Copyright © 2018 Youssef Moawad. All rights reserved.
//

#ifndef CircuitOptimiser_hpp
#define CircuitOptimiser_hpp

#include <stdio.h>
#include <pthread.h>
#include <unordered_map>

#include "QComputer.hpp"

using namespace std;

typedef pair<Tensor*, vector<QID>> ApplicableGate;
typedef pair<Tensor*, list_index> StateVectorApplicableGate;
typedef pair<vector<vector<QID>>, ApplicableGate> Step;

class CircuitOptimiser {
  
private:
  QComputer* comp;
  vector<ApplicableGate> gates;
  vector<vector<QID>> stateVectorsQubitIDs; // Used to track qubits combining
  
  list_index combineTwoQubits(QID q1, QID q2);
  list_index combineQubits(vector<QID> qIDs);
  list_index listIndexFromQID(QID qID);
  
  vector<Step> swapSteps(list_index stateVectorIndex, uint q1ID, uint q2ID);
  vector<Step> moveQubitSteps(list_index stateVectorIndex, uint src, uint des);
public:
  CircuitOptimiser(QComputer* comp, vector<ApplicableGate> gates);
  CircuitOptimiser(uint n, vector<ApplicableGate> gates);
  
  void addGates(vector<ApplicableGate> gates);
  void executeCircuit();
};


#endif /* CircuitOptimiser_hpp */
