//
//  Gates.cpp
//  SerialXcodeSimulator
//
//  Created by Youssef Moawad on 09/11/2018.
//  Copyright © 2018 Youssef Moawad. All rights reserved.
//

#include "Gates.hpp"

/************* Q Gates ***************/

key IKeys[2] = {make_pair(0, 0), make_pair(1, 1)};
cxd IVals[2] = {cxd(1), cxd(1)};
const SparseTensor I (2, 2, 2, IKeys, IVals);

key XKeys[2] = {make_pair(0, 1), make_pair(1, 0)};
cxd XVals[2] = {cxd(1), cxd(1)};
const SparseTensor X (2, 2, 2, XKeys, XVals);

key YKeys[2] = {make_pair(0, 0), make_pair(1, 1)};
cxd YVals[2] = {cxd(0,1), cxd(0,-1)};
const SparseTensor Y (2, 2, 2, YKeys, YVals);

key ZKeys[2] = {make_pair(0, 0), make_pair(1, 1)};
cxd ZVals[2] = {cxd(1), cxd(-1)};
const SparseTensor Z (2, 2, 2, ZKeys, ZVals);

key PKeys[2] = {make_pair(0, 0), make_pair(1, 1)};
cxd PVals[2] = {cxd(1), cxd(0,1)};
const SparseTensor P (2, 2, 2, PKeys, PVals);

key HKeys[4] = {make_pair(0, 0), make_pair(1, 1), make_pair(2, 3), make_pair(3, 2)};
cxd HVals[4] = {cxd(1/sqrt(2)), cxd(1/sqrt(2)), cxd(1/sqrt(2)), cxd(-1/sqrt(2))};
const SparseTensor H (2, 2, 4, HKeys, HVals);

key CNOTKeys[4] = {make_pair(0, 0), make_pair(1, 1), make_pair(2, 3), make_pair(3, 2)};
cxd CNOTVals[4] = {cxd(1), cxd(1), cxd(1), cxd(1)};
const SparseTensor CNOT (4, 4, 4, CNOTKeys, CNOTVals);

key SWAPKeys[4] = {make_pair(0, 0), make_pair(1, 2), make_pair(2, 1), make_pair(3, 3)};
cxd SWAPVals[4] = {cxd(1), cxd(1), cxd(1), cxd(1)};
const SparseTensor SWAP (4, 4, 4, SWAPKeys, SWAPVals);

key TOFFKeys[8] = {make_pair(0, 0), make_pair(1, 1), make_pair(2, 2), make_pair(3, 3), make_pair(4, 4), make_pair(5, 5), make_pair(6, 7), make_pair(7, 6)};
cxd TOFFVals[8] = {cxd(1), cxd(1), cxd(1), cxd(1), cxd(1), cxd(1), cxd(1), cxd(1)};
const SparseTensor TOFF (8, 8, 8, TOFFKeys, TOFFVals);

SparseTensor IGate() { return SparseTensor(I); }
SparseTensor XGate() { return SparseTensor(X); }
SparseTensor YGate() { return SparseTensor(Y); }
SparseTensor ZGate() { return SparseTensor(Z); }
SparseTensor PGate() { return SparseTensor(P); }
SparseTensor HGate() { return SparseTensor(H); }
SparseTensor CNOTGate() { return SparseTensor(CNOT); }
SparseTensor SWAPGate() { return SparseTensor(SWAP); }
SparseTensor TOFFGate() { return SparseTensor(TOFF); }

/************* END Q Gates ***************/

void GateCircuit(QComputer *comp, vector<QID> qIDs, SparseTensor t) {
  assert(t.colCount() == t.rowCount());
  int n = (int)log2(t.colCount());
  assert(qIDs.size() == n);
  assert(comp->numberOfQubits() >= n);
  
  comp->applyMultiGate(qIDs, t);
}

void IGate(QComputer *comp, vector<QID> qIDs) { GateCircuit(comp, qIDs, I); }
void XGate(QComputer *comp, vector<QID> qIDs) { GateCircuit(comp, qIDs, X); }
void YGate(QComputer *comp, vector<QID> qIDs) { GateCircuit(comp, qIDs, Y); }
void ZGate(QComputer *comp, vector<QID> qIDs) { GateCircuit(comp, qIDs, Z); }
void PGate(QComputer *comp, vector<QID> qIDs) { GateCircuit(comp, qIDs, P); }
void HGate(QComputer *comp, vector<QID> qIDs) { GateCircuit(comp, qIDs, H); }
void CNOTGate(QComputer *comp, vector<QID> qIDs) { GateCircuit(comp, qIDs, CNOT); }
void SWAPGate(QComputer *comp, vector<QID> qIDs) { GateCircuit(comp, qIDs, SWAP); }
void TOFFGate(QComputer *comp, vector<QID> qIDs) { GateCircuit(comp, qIDs, TOFF); }


