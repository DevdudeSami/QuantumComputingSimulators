//
//  QuantumErrorCorrection.hpp
//  SerialXcodeSimulator
//
//  Created by Youssef Moawad on 30/12/2018.
//  Copyright © 2018 Youssef Moawad. All rights reserved.
//

#ifndef QuantumErrorCorrection_hpp
#define QuantumErrorCorrection_hpp

#include <stdio.h>
#include <random>
#include "QComputer.hpp"
#include "CircuitOptimiser.hpp"

using namespace std;

vector<ApplicableGate> BitFlipEncodeGates(vector<QID> qIDs);
vector<ApplicableGate> BitFlipDecodeGates(vector<QID> qIDs);
void BitFlipCodeTest(QComputer *comp, float flipProbability);

vector<ApplicableGate> PhaseFlipEncodeGates(vector<QID> qIDs);
vector<ApplicableGate> PhaseFlipDecodeGates(vector<QID> qIDs);
void PhaseFlipCodeTest(QComputer *comp, float flipProbability);

vector<ApplicableGate> ShorEncodeGates(vector<QID> qIDs);
vector<ApplicableGate> ShorDecodeGates(vector<QID> qIDs);
void ShorCodeTest(QComputer *comp, float flipProbability);




#endif /* QuantumErrorCorrection_hpp */
