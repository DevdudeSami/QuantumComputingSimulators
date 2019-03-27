//
//  Accumulator.cpp
//  MatrixBasedSimulator
//
//  Created by Youssef Moawad on 04/12/2018.
//  Copyright © 2018 Youssef Moawad. All rights reserved.
//

#include "Accumulator.hpp"


states_count takeMeasurements(StateVector state, unsigned int n, int *qID) {
  states_count results;
  
  if(qID == nullptr) {
    
    for(int i = 0; i < n; i++) {
      string measurement = state.measure();
      if (results.find(measurement) == results.end()) results[measurement] = 1;
      else results[measurement]++;
    }
  }
  
  return results;
}

string takeMeasurementsInString(StateVector state, unsigned int n, int *qID) {
  states_count measurements = takeMeasurements(state, n, qID);
  string results = "{";
  
  for (auto m: measurements) {
    results += "\"" + m.first + "\": " + to_string(m.second) + ", ";
  }
  
  results.pop_back();
  results.pop_back();
  
  results += "}";
  
  return results;
}
