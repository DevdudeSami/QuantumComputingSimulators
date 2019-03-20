//
//  Algorithms.hpp
//  SerialXcodeSimulator
//
//  Created by Youssef Moawad on 09/12/2018.
//  Copyright © 2018 Youssef Moawad. All rights reserved.
//

#ifndef Algorithms_hpp
#define Algorithms_hpp

#include <stdio.h>
#include "QComputer.hpp"

using namespace std;

void QuantumFourierTransform(QComputer *comp, vector<QID> qIDs);
void InverseQuantumFourierTransform(QComputer *comp, vector<QID> qIDs);
vector<ApplicableGate> QuantumFourierTransformGates(vector<QID> qIDs);
vector<ApplicableGate> InverseQuantumFourierTransformGates(vector<QID> qIDs);

#endif /* Algorithms_hpp */