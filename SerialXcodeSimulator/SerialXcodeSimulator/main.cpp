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

  QuantumLanguageInterpreter qli ("/Users/youssefmoawad/Documents/University/Year 4/Project/SerialXcodeSimulator/SerialXcodeSimulator/QLITest/", "main.qli");
  cout << qli.execute() << endl;

//  cout << CircuitOptimisedNQubitCuccaroAdder(9, "000000001", "000000001") << endl;
  
  
//  cout << GroversSearch(10, {2,5}, 2) << endl;
  
//  QComputer comp (18);
//
//  comp.entangleQubits({0,9});
//
//  vector<ApplicableGate> firstEncode = ShorEncodeGates({0,1,2,3,4,5,6,7,8});
//  vector<ApplicableGate> firstDecode = ShorDecodeGates({0,1,2,3,4,5,6,7,8});
//  vector<ApplicableGate> secondEncode = ShorEncodeGates({9,10,11,12,13,14,15,16,17});
//  vector<ApplicableGate> secondDecode = ShorDecodeGates({9,10,11,12,13,14,15,16,17});
//
//
//  vector<ApplicableGate> gates;
//
//  gates.insert(gates.end(), firstEncode.begin(), firstEncode.end());
//  gates.insert(gates.end(), secondEncode.begin(), secondEncode.end());
//
//  gates.push_back(HAGate({0}));
//  gates.push_back(ZAGate({9}));
//
//  gates.insert(gates.end(), firstDecode.begin(), firstDecode.end());
//  gates.insert(gates.end(), secondDecode.begin(), secondDecode.end());
//
//  CircuitOptimiser co (&comp, gates);
//  co.executeCircuit();
//
//  cout << takeMeasurementsInString(comp, 10, nullptr) << endl;
  
  auto end = chrono::high_resolution_clock::now();
  auto diff = end - start;
  cout << "--- end time: " << (chrono::duration <double, milli> (diff).count())/1000 << " s" << endl;

  return 0;
}
