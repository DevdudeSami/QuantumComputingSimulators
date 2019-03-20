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
#include "Tensor.hpp"

using namespace std;

class StateVector {
private:
  unsigned long n;
  Tensor amplitudes;
  vector<int> qIDs;
public:
  StateVector(Tensor amps, vector<int> ids);
  StateVector(uint n);
  vector<double> probabilities();
  uint numberOfQubits();
  void applyGate(Tensor t);
  void applyNGate(Tensor t, vector<unsigned int> qIDs);
  string measure();
  StateVector combineWith(StateVector v);
  Tensor prepareOperator(Tensor t, vector<int> indices);
  void swap(unsigned int q1ID, unsigned int q2ID);
};


#endif /* StateVector_hpp */
