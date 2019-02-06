//
//  QRegister.cpp
//  NoMatrixSimulator
//
//  Created by Youssef Moawad on 05/02/2019.
//  Copyright © 2019 Youssef Moawad. All rights reserved.
//

#include "QRegister.hpp"

QRegister::QRegister(uint n) : n{n} {
  for(QID i = 0; i < n; i++) {
    QID qIDs[1] = {i};
    StateVector v (1, qIDs);
    
    vectors.push_back(v);
  }
}

void QRegister::applySingleGate(QID qID, Gate gate) {
  vectors[listIndexFromQID(qID)].applySingleGate(qID, gate);
}

void QRegister::applyCnGate(vector<QID> controls, QID qID, Gate gate) {
  list_index combinedIndex = 0;
  for(QID c : controls) combinedIndex = combineTwoQubits(c, qID);
  
  vectors[combinedIndex].applyCnGate(controls, qID, gate);
}

list_index QRegister::combineTwoQubits(QID q1ID, QID q2ID) {
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

list_index QRegister::listIndexFromQID(QID qID) {
  for(list_index i = 0; i < vectors.size(); i++) {
    QID* qubitIDs = vectors[i].qubitIDs();
    if (find(qubitIDs, qubitIDs+vectors[i].numberOfQubits(), qID) != qubitIDs+vectors[i].numberOfQubits()) {
      return i;
    }
  }
  
  throw exception();
}

string QRegister::measure() {
  vector<char> result (' ', n);
  
  for(StateVector v: vectors) {
    string measurement = v.measure();
    QID* qIDs = v.qubitIDs();
    for(int i = 0; i < measurement.length(); i++) {
      QID qID = qIDs[i];
      result[qID] = measurement[i];
    }
  }
  
  return "|" + string(result.begin(), result.end()) + ">";
}
