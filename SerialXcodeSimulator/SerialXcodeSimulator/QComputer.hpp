//
//  QComputer.hpp
//  SerialXcodeSimulator
//
//  Created by Youssef Moawad on 04/12/2018.
//  Copyright © 2018 Youssef Moawad. All rights reserved.
//

#ifndef QComputer_hpp
#define QComputer_hpp

#include <stdio.h>
#include "StateVector.hpp"
#include "SparseTensor.hpp"
#include "Gates.hpp"

using namespace std;

typedef unsigned int QID;
typedef unsigned int list_index;

class QComputer {
private:
  vector<StateVector> vectors;
  unsigned int n;
public:
  QComputer(vector<StateVector> vectors);
  QComputer(string s);
  QComputer(unsigned int n);
  
  vector<StateVector> stateVectors();
  StateVector stateVector();
  string measure();
  
  void applySingleGate(QID qID, SparseTensor gate);
  void applySingleGateToMutlipleQubits(vector<QID> qIDs, SparseTensor gate);
  void applyMultiGate(vector<QID> qIDs, SparseTensor gate);
  
  /// The first qubit in the vector is used as the control.
  void entangleQubits(vector<QID> qIDs);
  
  list_index combineTwoQubits(QID q1ID, QID q2ID);
  list_index combineQubits(vector<QID> qIDs);
  StateVector stateVectorWithQID(QID qID);
  list_index listIndexFromQID(QID qID);
  vector<QID> allQubits();
};



#endif /* QComputer_hpp */
