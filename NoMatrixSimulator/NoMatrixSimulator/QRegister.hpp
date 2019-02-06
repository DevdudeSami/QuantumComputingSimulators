//
//  QRegister.hpp
//  NoMatrixSimulator
//
//  Created by Youssef Moawad on 05/02/2019.
//  Copyright © 2019 Youssef Moawad. All rights reserved.
//

#ifndef QRegister_hpp
#define QRegister_hpp

#include <stdio.h>
#include <complex>
#include <vector>
#include <random>
#include <omp.h>

#include "Typedefs.hpp"
#include "StateVector.hpp"

using namespace std;

class QRegister {
  
private:
  uint n;
  vector<StateVector> vectors;

  list_index listIndexFromQID(QID qID);
  
public:
  QRegister(uint n);

  list_index combineTwoQubits(QID q1ID, QID q2ID);
  
  void applySingleGate(QID qID, Gate gate);
  void applyCnGate(vector<QID> controls, QID qID, Gate gate);
  
  string measure();
};


#endif /* QRegister_hpp */
