//
//  Accumulator.hpp
//  NoMatrixSimulator
//
//  Created by Youssef Moawad on 05/02/2019.
//  Copyright © 2019 Youssef Moawad. All rights reserved.
//

#ifndef Accumulator_hpp
#define Accumulator_hpp

#include <stdio.h>
#include <map>
#include "QRegister.hpp"

using namespace std;

typedef  map<string, unsigned int> states_count;

states_count takeMeasurements(QRegister reg, unsigned int n);
string takeMeasurementsInString(QRegister reg, unsigned int n);

#endif /* Accumulator_hpp */

