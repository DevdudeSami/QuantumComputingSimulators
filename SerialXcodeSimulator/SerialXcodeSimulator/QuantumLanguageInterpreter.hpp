//
//  QuantumLanguageInterpreter.hpp
//  SerialXcodeSimulator
//
//  Created by Youssef Moawad on 18/01/2019.
//  Copyright © 2019 Youssef Moawad. All rights reserved.
//

#ifndef QuantumLanguageInterpreter_hpp
#define QuantumLanguageInterpreter_hpp

#include <stdio.h>
#include <fstream>
#include "CircuitOptimiser.hpp"
#include "Accumulator.hpp"
#include <boost/algorithm/string.hpp>
#include <unordered_map>
#include "tinyexpr/tinyexpr.h"

using namespace std;

class QuantumLanguageInterpreter {
private:
  string lookUpFolder;
  ifstream file;
  QComputer *comp;
  unordered_map<string, vector<ApplicableGate>> userDefinedGates;
  vector<ApplicableGate> gates;
  vector<QID> qubitsToReturn;
  
  Tensor *gateFor(string g);
  
  void handleLink(ifstream *file);
  void handleLine(vector<string> splitString, vector<ApplicableGate> *gates, ifstream *file);
  void handleLoop(string symbol, int start, int end, ifstream *file, vector<ApplicableGate> *gates);
  void selfDefineGate(string gateName, ifstream *file);
public:
  QuantumLanguageInterpreter(string lookUpFolder, string filename);
  string execute();
};


#endif /* QuantumLanguageInterpreter_hpp */
