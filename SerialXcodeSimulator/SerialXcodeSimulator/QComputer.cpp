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
  for(int i = 0; i < n; i++) {
    
    key keys[1] = {make_pair(0,0)};
    cxd vals[1] = {cxd(1)};
    
    SparseTensor amps (1, 2, 1, keys, vals);
    StateVector v (amps, vector<int>({i}));
    
    vectors.push_back(v);
  }
}

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
