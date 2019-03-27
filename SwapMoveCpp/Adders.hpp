//
//  Adders.hpp
//  MatrixBasedSimulator
//
//  Created by Youssef Moawad on 29/12/2018.
//  Copyright © 2018 Youssef Moawad. All rights reserved.
//

#ifndef Adders_hpp
#define Adders_hpp

#include <stdio.h>
#include "QComputer.hpp"
#include "Algorithms.hpp"

using namespace std;

void HalfAdderCircuit(QComputer *comp, vector<QID> qIDs);
string HalfAdder(uint A, uint B);

void FullAdderCircuit(QComputer *comp, vector<QID> qIDs);
string FullAdder(uint A, uint B, uint C);

void MAJCircuit(QComputer *comp, vector<QID> qIDs);
void UMACircuit(QComputer *comp, vector<QID> qIDs);
string SixQubitRippleCarryAdder(string A, string B);
string AltSixQubitRippleCarryAdder(string A, string B);

string NBitCuccaroAdder(uint n, string A, string B);

#endif /* Adders_hpp */
