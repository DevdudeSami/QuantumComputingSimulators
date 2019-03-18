//
//  main.cpp
//  NoMatrixSimulator
//
//  Created by Youssef Moawad on 05/02/2019.
//  Copyright © 2019 Youssef Moawad. All rights reserved.
//

#include <iostream>

#include "QRegister.hpp"

#include "Gates.hpp"
#include "Accumulator.hpp"
#include "Adders.hpp"
#include "Algorithms.hpp"

using namespace std;

int main(int argc, const char * argv[]) {
  auto start = chrono::high_resolution_clock::now();
  
  cout << GroversSearch(15, {2,5,8}, 180) << endl;
  
//  QRegister reg (10);
//  reg.applyMultiGate(reg.allQubits(), QFT(10));
//  QuantumFourierTransform(&reg, reg.allQubits());

  // rubbish op to combine everything
//  reg.applyCnGate({1,2}, 0, X());
//
//  reg.applySingleGate(0, X());
//
//  reg.applyMultiGate({0,1}, SWAP());
////  reg.applyMultiGate({1,2}, SWAP());
//  reg.applyMultiGate({0,2}, SWAP());


//  cout << takeMeasurementsInString(reg, 1000) << endl;

//  uint* qIDs = new uint[4];
//  qIDs[0] = 0;
//  qIDs[1] = 1;
//  qIDs[2] = 2;
//  qIDs[3] = 3;
//  StateVector v (4,qIDs);
//  v.applySingleGate(0, X());
  
//  v.makeAdjacent({1,0});
  
//  v.applyMultiGate({2,0}, SWAP());
//  v.applyMultiGate({1,2}, SWAP());
//  v.applyMultiGate({3,1}, SWAP());
//  v.applyMultiGate({2,1}, SWAP());
  
//  cout << v.measure() << endl;
  
//  cout << NBitCuccaroAdder(7, "0000001", "0000001") << endl;
////
//  for(int i = 1; i < reg.numberOfQubits(); i++) {
//    reg.combineTwoQubits(0, i);
//  }
//
//  reg.applySingleGate(0, H());
//
//  #pragma omp parallel for
//  for(int i = 1; i < reg.numberOfQubits(); i++) {
//    reg.applyCnGate({0}, i, X());
//  }
  
//  cout << reg.measure() << endl;
  
//  cout << takeMeasurementsInString(reg, 1000) << endl;
  

  auto end = chrono::high_resolution_clock::now();
  auto diff = end - start;
  cout << "--- end time: " << (chrono::duration <double, milli> (diff).count())/1000 << " s" << endl;
  
  return 0;
}
