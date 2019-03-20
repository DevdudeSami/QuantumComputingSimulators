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
#include "tinyexpr.h"

using namespace std;

class QuantumLanguageInterpreter {
private:
  string lookUpFolder;
  ifstream file;
  QComputer *comp;
  unordered_map<string, vector<string>> userDefinedGates;
  unordered_map<string, unordered_map<string, string>> userDefinedValues;
  vector<ApplicableGate> gates;
  vector<QID> qubitsToReturn;
  string gateBeingExecuted;
  
  Tensor *gateFor(string g);
  
  string evalFromDefinedValues(string gate, string expr);
  void handleLink(ifstream *file);
  void handleLine(vector<string> splitString, vector<ApplicableGate> *gates, ifstream *file);
  void handleLoop(string symbol, int start, int end, int step, ifstream *file, vector<ApplicableGate> *gates, vector<string> *lines);
  void handleLet(string gate, string symbol, string expr);
  void selfDefineGate(string gateName, ifstream *file);
public:
  QuantumLanguageInterpreter(string lookUpFolder, string filename);
  string execute();
};


#endif /* QuantumLanguageInterpreter_hpp */
