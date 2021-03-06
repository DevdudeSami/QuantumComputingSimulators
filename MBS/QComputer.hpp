//
//  QComputer.hpp
//  MatrixBasedSimulator
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
  
  unsigned int numberOfQubits();
  vector<StateVector> stateVectors();
  StateVector stateVector();
  string measure();
  
  void applySingleGate(QID qID, Tensor* gate);
  void applySingleGateToMutlipleQubits(vector<QID> qIDs, Tensor* gate);
  void applyMultiGate(vector<QID> qIDs, Tensor* gate);
  void applyPreparedGate(list_index stateVectorIndex, Tensor* gate);
  
  /// The first qubit in the vector is used as the control.
  void entangleQubits(vector<QID> qIDs);
  
  void flipQubit(QID qID);
  
  list_index combineTwoQubits(QID q1ID, QID q2ID);
  list_index combineQubits(vector<QID> qIDs);
  StateVector stateVectorWithQID(QID qID);
  list_index listIndexFromQID(QID qID);
  vector<QID> allQubits();
  
  void setStateVectorQIDs(list_index stateVectorIndex, vector<QID> qIDs);
};



#endif /* QComputer_hpp */
