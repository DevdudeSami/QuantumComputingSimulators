//
//  StateVector.hpp
//  SerialXcodeSimulator
//
//  Created by Youssef Moawad on 09/11/2018.
//  Copyright © 2018 Youssef Moawad. All rights reserved.
//

#ifndef StateVector_hpp
#define StateVector_hpp

#include <stdio.h>
#include "SparseTensor.hpp"

using namespace std;

class StateVector {
private:
  unsigned long n;
  SparseTensor amplitudes;
  vector<unsigned int> qIDs;
public:
  StateVector(SparseTensor amps, vector<unsigned int> ids);
  vector<double> probabilities();
  unsigned long numberOfQubits();
  vector<unsigned int> qubitIDs();
  void applyGate(Tensor* t);
  void applyNGate(Tensor* t, vector<unsigned int> qIDs);
  string measure();
  StateVector combineWith(StateVector v);
  Tensor* prepareOperator(Tensor *t, vector<unsigned int> indices);
  Tensor* prepareOperatorToRunAtStart(Tensor* t, uint numberOfAppliedQubits);
  Tensor* prepareOperatorToRunInMiddle(Tensor* t, uint startingIndex);
  void swap(unsigned int q1ID, unsigned int q2ID);
  
  void setQIDs(vector<unsigned int> qIDs);
};


#endif /* StateVector_hpp */
