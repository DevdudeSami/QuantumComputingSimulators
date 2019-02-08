//
//  Algorithms.cpp
//  NoMatrixSimulator
//
//  Created by Youssef Moawad on 07/02/2019.
//  Copyright © 2019 Youssef Moawad. All rights reserved.
//

#include "Algorithms.hpp"

void QuantumFourierTransform(QRegister *reg, vector<QID> qIDs) {
  for(int i = 0; i < qIDs.size(); i++) {
    reg->applySingleGate(qIDs[i], H());
    
    for(int j = i+1; j < qIDs.size(); j++) {
      reg->applyCnGate({qIDs[j]}, qIDs[i], Rm(j-i+1));
    }
  }
}

void InverseQuantumFourierTransform(QRegister *reg, vector<QID> qIDs) {
  for(int i = qIDs.size() - 1; i >= 0; i--) {
    for(int j = qIDs.size() - 1; j > i; j--) {
      reg->applyCnGate({qIDs[j]}, qIDs[i], Rm(-(j-i+1)));
    }
    
    reg->applySingleGate(qIDs[i], H());
  }
}
