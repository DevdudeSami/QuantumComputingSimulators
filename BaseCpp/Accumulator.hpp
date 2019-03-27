//
//  Accumulator.hpp
//  MatrixBasedSimulator
//
//  Created by Youssef Moawad on 04/12/2018.
//  Copyright © 2018 Youssef Moawad. All rights reserved.
//

#ifndef Accumulator_hpp
#define Accumulator_hpp

#include <stdio.h>
#include <map>
#include "StateVector.hpp"

using namespace std;

typedef  map<string, unsigned int> states_count;

states_count takeMeasurements(StateVector state, unsigned int n, int *qID);
string takeMeasurementsInString(StateVector state, unsigned int n, int *qID);

#endif /* Accumulator_hpp */
