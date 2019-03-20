//
//  StateVector.hpp
//  NoMatrixSimulator
//
//  Created by Youssef Moawad on 05/02/2019.
//  Copyright © 2019 Youssef Moawad. All rights reserved.
//

#ifndef StateVector_hpp
#define StateVector_hpp

#include <stdio.h>
#include <complex>
#include <vector>
#include <random>
#include <omp.h>

#include "Typedefs.hpp"
#include "Gates.hpp"

using namespace std;

class StateVector {
  
private:
  uint n;
  cxd* amplitudes;
  uint* qIDs;
  
  void swapAdjacentQubits(list_index q1Index);
  void move(list_index srcIndex, list_index desIndex);
  list_index makeAdjacent(vector<QID> qIDs);
  
public:
  StateVector(uint n, uint* qIDs);
  
  
  void setAmplitudes(cxd* newAmplitudes);
  
  double* probabilities();
  
  uint numberOfQubits();
  uint* qubitIDs();
  
  void applySingleGate(QID qID, Gate gate);
  void applyMultiGate(vector<QID> qIDs, Gate gate);
  void applyCnGate(vector<QID> controls, QID qID, Gate gate);
  
  string measure();
  
  StateVector combineWith(StateVector v);
};


#endif /* StateVector_hpp */
