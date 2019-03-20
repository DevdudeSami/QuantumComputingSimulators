//
//  Gates.hpp
//  SerialXcodeSimulator
//
//  Created by Youssef Moawad on 09/11/2018.
//  Copyright © 2018 Youssef Moawad. All rights reserved.
//

#ifndef Gates_hpp
#define Gates_hpp

#include <stdio.h>
#include "Tensor.hpp"
#include "StateVector.hpp"

typedef uint QID;

Tensor IGate();
Tensor XGate();
Tensor YGate();
Tensor ZGate();
Tensor HGate();

Tensor CNOTGate();
Tensor SWAPGate();

Tensor TOFFGate();

void GateCircuit(StateVector *comp, vector<QID> qIDs, Tensor t);

void IGate(StateVector *comp, vector<QID> qIDs);
void XGate(StateVector *comp, vector<QID> qIDs);
void YGate(StateVector *comp, vector<QID> qIDs);
void ZGate(StateVector *comp, vector<QID> qIDs);
void HGate(StateVector *comp, vector<QID> qIDs);
void CNOTGate(StateVector *comp, vector<QID> qIDs);
void SWAPGate(StateVector *comp, vector<QID> qIDs);
void TOFFGate(StateVector *comp, vector<QID> qIDs);


#endif /* Gates_hpp */
