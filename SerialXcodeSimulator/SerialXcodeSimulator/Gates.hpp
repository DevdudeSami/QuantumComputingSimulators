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
#include "QComputer.hpp"

using namespace std;

class QComputer;
typedef unsigned int QID;

SparseTensor IGate();
SparseTensor XGate();
SparseTensor YGate();
SparseTensor ZGate();
SparseTensor PGate();
SparseTensor HGate();

SparseTensor CNOTGate();
SparseTensor SWAPGate();

SparseTensor TOFFGate();

SparseTensor CRm(uint m);

void GateCircuit(QComputer *comp, vector<QID> qIDs, SparseTensor t);

void IGate(QComputer *comp, vector<QID> qIDs);
void XGate(QComputer *comp, vector<QID> qIDs);
void YGate(QComputer *comp, vector<QID> qIDs);
void ZGate(QComputer *comp, vector<QID> qIDs);
void PGate(QComputer *comp, vector<QID> qIDs);
void HGate(QComputer *comp, vector<QID> qIDs);
void CNOTGate(QComputer *comp, vector<QID> qIDs);
void SWAPGate(QComputer *comp, vector<QID> qIDs);
void TOFFGate(QComputer *comp, vector<QID> qIDs);
void CRmGate(uint m, QComputer *comp, vector<QID> qIDs);

#endif /* Gates_hpp */
