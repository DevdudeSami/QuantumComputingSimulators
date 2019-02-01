//
//  main.cpp
//  SerialXcodeSimulator
//
//  Created by Youssef Moawad on 02/11/2018.
//  Copyright © 2018 Youssef Moawad. All rights reserved.
//

#include <iostream>
#include <time.h>
//#include <omp.h>
#include <mpi.h>

#include "DenseTensor.hpp"
#include "Gates.hpp"
#include "StateVector.hpp"
#include "SparseTensor.hpp"
#include "Accumulator.hpp"
#include "QComputer.hpp"
#include "Algorithms.hpp"
#include "Adders.hpp"
#include "QuantumErrorCorrection.hpp"
#include "QuantumLanguageInterpreter.hpp"

using namespace std;

int main(int argc, char const *argv[]) {
  auto start = chrono::high_resolution_clock::now();

//  QuantumLanguageInterpreter qli ("/Users/youssefmoawad/Documents/University/Year 4/Project/SerialXcodeSimulator/SerialXcodeSimulator/QLITest/", "main.qli");
//  cout << qli.execute() << endl;
//
//  QComputer comp (3);
//  comp.combineQubits(comp.allQubits());
//  CircuitOptimisedQuantumFourierTransform(&comp, comp.allQubits());
//  comp.applySingleGate(1, new SparseTensor(ZGate()));
//  comp.applySingleGate(0, new SparseTensor(ZGate()));
//  comp.applySingleGate(2, new SparseTensor(ZGate()));
//  CircuitOptimisedInverseQuantumFourierTransform(&comp, {0,1});
//
//  for(auto v: comp.stateVectors()) {
//    for(auto p: v.probabilityAmplitudes()) {
//      cout << p << endl;
//    }
//    cout << "----" << endl;
//  }
//
//  cout << takeMeasurementsInString(comp, 1000, nullptr);
  
  cout << CircuitOptimisedNBitQFTAdder(7, "0101010", "0111000") << endl;
  cout << CircuitOptimisedNQubitCuccaroAdder(7, "0101010", "0111000") << endl;
  
//  QComputer comp (3);
//  comp.flipQubit(0);
//  Teleport(&comp, 0, 1, 2);
//
//  cout << takeMeasurementsInString(comp, 1000, nullptr) << endl;
  
  auto end = chrono::high_resolution_clock::now();
  auto diff = end - start;
  cout << "--- end time: " << (chrono::duration <double, milli> (diff).count())/1000 << " s" << endl;

  return 0;
}
