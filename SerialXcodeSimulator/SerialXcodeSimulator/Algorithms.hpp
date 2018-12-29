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
#include "CircuitOptimiser.hpp"

using namespace std;

void HalfAdderCircuit(QComputer *comp, vector<QID> qIDs);
string HalfAdder(uint A, uint B);

void FullAdderCircuit(QComputer *comp, vector<QID> qIDs);
string FullAdder(uint A, uint B, uint C);

void MAJCircuit(QComputer *comp, vector<QID> qIDs);
void UMACircuit(QComputer *comp, vector<QID> qIDs);
string SixQubitRippleCarryAdder(string A, string B);
string AltSixQubitRippleCarryAdder(string A, string B);
string CircuitOptimisedAltSixQubitRippleCarryAdder(string A, string B);

void QuantumFourierTransform(QComputer *comp, vector<QID> qIDs);
void CircuitOptimisedQuantumFourierTransform(QComputer *comp, vector<QID> qIDs);

void CircuitOptimisedTwoQubitEntanglement(QComputer *comp, vector<QID> qIDs);

#endif /* Algorithms_hpp */
