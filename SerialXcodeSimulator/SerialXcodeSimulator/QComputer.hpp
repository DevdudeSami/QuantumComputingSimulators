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

using namespace std;

class QComputer {
private:
  vector<StateVector> vectors;
  unsigned int n;
public:
  QComputer(vector<StateVector> vectors);
  QComputer(string s);
  QComputer(unsigned int n);
  
  StateVector stateVector();
  string measure();
};



#endif /* QComputer_hpp */
