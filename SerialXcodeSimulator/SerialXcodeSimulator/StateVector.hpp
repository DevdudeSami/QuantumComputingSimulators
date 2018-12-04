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
  vector<int> qIDs;
public:
  StateVector(SparseTensor amps, vector<int> ids);
  vector<double> probabilities();
  void applyGate(SparseTensor t);
  void applyNGate(SparseTensor t, vector<int> qIDs);
  string measure();
  StateVector combineWith(StateVector v);
  SparseTensor prepareOperator(SparseTensor t, vector<int> indices);
  void swap(unsigned int q1ID, unsigned int q2ID);
};


#endif /* StateVector_hpp */
