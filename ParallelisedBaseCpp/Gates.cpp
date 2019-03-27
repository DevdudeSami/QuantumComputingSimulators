//
//  Gates.cpp
//  MatrixBasedSimulator
//
//  Created by Youssef Moawad on 09/11/2018.
//  Copyright © 2018 Youssef Moawad. All rights reserved.
//

#include "Gates.hpp"

/************* Q Gates ***************/
const Tensor I ({{cxd(1),cxd(0)},{0,1}});

const Tensor X ({{cxd(0),1},{1,0}});

const Tensor Y ({{cxd(0),cxd(0,-1)},{cxd(0,1),0}});

const Tensor Z ({{cxd(1),cxd(0)},{0,-1}});

const Tensor H ({{cxd(1/sqrt(2)), cxd(1/sqrt(2))}, {cxd(1/sqrt(2)), cxd(-1/sqrt(2))}});

const Tensor CNOT ({{1,0,0,0},{0,1,0,0},{0,0,0,cxd(1)},{0,0,1,0}});

const Tensor SWAP ({{1,0,0,0},{0,0,1,0},{0,0,1,0},{0,0,0,cxd(1)}});

const Tensor TOFF ({
  {1,0,0,0,0,0,0,0},
  {0,1,0,0,0,0,0,0},
  {0,0,1,0,0,0,0,0},
  {0,0,0,1,0,0,0,0},
  {0,0,0,0,1,0,0,0},
  {0,0,0,0,0,1,0,0},
  {0,0,0,0,0,0,0,1},
  {0,0,0,0,0,0,cxd(1),0}
});

Tensor IGate() { return Tensor(I); }
Tensor XGate() { return Tensor(X); }
Tensor YGate() { return Tensor(Y); }
Tensor ZGate() { return Tensor(Z); }
Tensor HGate() { return Tensor(H); }
Tensor CNOTGate() { return Tensor(CNOT); }
Tensor SWAPGate() { return Tensor(SWAP); }
Tensor TOFFGate() { return Tensor(TOFF); }

/************* END Q Gates ***************/

void GateCircuit(StateVector *comp, vector<QID> qIDs, Tensor t) {
  assert(t.colCount() == t.rowCount());
  int n = (int)log2(t.colCount());
  assert(qIDs.size() == n);
  assert(comp->numberOfQubits() >= n);
  
  comp->applyNGate(t, qIDs);
}

void IGate(StateVector *comp, vector<QID> qIDs) { GateCircuit(comp, qIDs, I); }
void XGate(StateVector *comp, vector<QID> qIDs) { GateCircuit(comp, qIDs, X); }
void YGate(StateVector *comp, vector<QID> qIDs) { GateCircuit(comp, qIDs, Y); }
void ZGate(StateVector *comp, vector<QID> qIDs) { GateCircuit(comp, qIDs, Z); }
void HGate(StateVector *comp, vector<QID> qIDs) { GateCircuit(comp, qIDs, H); }
void CNOTGate(StateVector *comp, vector<QID> qIDs) { GateCircuit(comp, qIDs, CNOT); }
void SWAPGate(StateVector *comp, vector<QID> qIDs) { GateCircuit(comp, qIDs, SWAP); }
void TOFFGate(StateVector *comp, vector<QID> qIDs) { GateCircuit(comp, qIDs, TOFF); }

