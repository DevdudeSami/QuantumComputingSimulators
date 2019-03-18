//
//  Algorithms.hpp
//  NoMatrixSimulator
//
//  Created by Youssef Moawad on 07/02/2019.
//  Copyright © 2019 Youssef Moawad. All rights reserved.
//

#ifndef Algorithms_hpp
#define Algorithms_hpp

#include <stdio.h>
#include <vector>
#include <complex>
#include "Typedefs.hpp"
#include "QRegister.hpp"
#include "Gates.hpp"

using namespace std;

void QuantumFourierTransform(QRegister *reg, vector<QID> qIDs);
void InverseQuantumFourierTransform(QRegister *reg, vector<QID> qIDs);

string GroversSearch(uint n, vector<uint> markedStates, uint iters);

#endif /* Algorithms_hpp */
