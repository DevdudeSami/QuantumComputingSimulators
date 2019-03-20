//
//  Accumulator.cpp
//  NoMatrixSimulator
//
//  Created by Youssef Moawad on 05/02/2019.
//  Copyright © 2019 Youssef Moawad. All rights reserved.
//

#include "Accumulator.hpp"

states_count takeMeasurements(QRegister reg, unsigned int n) {
  states_count results;

  for(int i = 0; i < n; i++) {
    string measurement = reg.ketMeasure();
    if (results.find(measurement) == results.end()) results[measurement] = 1;
    else results[measurement]++;
  }

  return results;
}

string takeMeasurementsInString(QRegister reg, unsigned int n) {
  states_count measurements = takeMeasurements(reg, n);
  string results = "{";

  for (auto m: measurements) {
    results += "\"" + m.first + "\": " + to_string(m.second) + ", ";
  }

  results.pop_back();
  results.pop_back();

  results += "}";

  return results;
}
