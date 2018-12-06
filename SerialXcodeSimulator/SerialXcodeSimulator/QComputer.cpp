//
//  QComputer.cpp
//  SerialXcodeSimulator
//
//  Created by Youssef Moawad on 04/12/2018.
//  Copyright © 2018 Youssef Moawad. All rights reserved.
//

#include "QComputer.hpp"

QComputer::QComputer(vector<StateVector> vectors) : vectors{vectors} {
  n = 0;
  
  for(StateVector v: vectors) {
    n += v.numberOfQubits();
  }
}

QComputer::QComputer(unsigned int n) : n{n} {
  for(QID i = 0; i < n; i++) {
    
    key keys[1] = {make_pair(0,0)};
    cxd vals[1] = {cxd(1)};
    
    SparseTensor amps (1, 2, 1, keys, vals);
    StateVector v (amps, vector<QID>({i}));
    
    vectors.push_back(v);
  }
}

vector<StateVector> QComputer::stateVectors() { return vectors; }

StateVector QComputer::stateVector() {
  assert(vectors.size() > 0);
  
  StateVector v = vectors[0];
  
  for(auto it = ++vectors.begin(); it != vectors.end(); ++it) {
    v = v.combineWith(*it);
  }
  
  return v;
}

string QComputer::measure() {
  string result = "|";
  
  for(StateVector v: vectors)
    result += v.measure();
  
  result += ">";
  
  return result;
}

list_index QComputer::combineTwoQubits(QID q1ID, QID q2ID) {
  list_index q1Index = listIndexFromQID(q1ID);
  list_index q2Index = listIndexFromQID(q2ID);
  
  // if already combined, do nothing
  if(q1Index == q2Index) return q1Index;
  
  StateVector combinedState = vectors[q1Index].combineWith(vectors[q2Index]);
  unsigned int indexOfCombinedState = min(q1Index, q2Index);
  vectors[indexOfCombinedState] = combinedState;
  vectors.erase(vectors.begin() + max(q1Index, q2Index));
  return indexOfCombinedState;
}

list_index QComputer::combineQubits(vector<QID> qIDs) {
  list_index finalCombinedStateIndex = listIndexFromQID(qIDs[0]);
  QID firstQID = qIDs[0];
  for(QID qID: qIDs) {
    if(qID == firstQID) continue;
    finalCombinedStateIndex = combineTwoQubits(firstQID, qID);
  }
  
  return finalCombinedStateIndex;
}

void QComputer::applySingleGate(QID qID, SparseTensor gate) {
  vectors[listIndexFromQID(qID)].applyNGate(gate, vector<QID>({qID}));
}

void QComputer::applySingleGateToMutlipleQubits(vector<QID> qIDs, SparseTensor gate) {
  for(QID qID: qIDs) {
    applySingleGate(qID, gate);
  }
}

void QComputer::applyMultiGate(vector<QID> qIDs, SparseTensor gate) {
  list_index combinedStateIndex = combineQubits(qIDs);
  vectors[combinedStateIndex].applyNGate(gate, qIDs);
}

StateVector QComputer::stateVectorWithQID(QID qID) {
  return vectors[listIndexFromQID(qID)];
}

unsigned int QComputer::listIndexFromQID(QID qID) {
  for(list_index i = 0; i < vectors.size(); i++) {
    vector<QID> qubitIDs = vectors[i].qubitIDs();
    if (find(qubitIDs.begin(), qubitIDs.end(), qID) != qubitIDs.end()) {
      return i;
    }
  }
  
  throw exception();
}
