//
//  QuantumLanguageInterpreter.hpp
//  MatrixBasedSimulator
//
//  Created by Youssef Moawad on 18/01/2019.
//  Copyright © 2019 Youssef Moawad. All rights reserved.
//

#ifndef QuantumLanguageInterpreter_hpp
#define QuantumLanguageInterpreter_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <unordered_map>
#include "Accumulator.hpp"
#include "tinyexpr.h"

typedef uint QID;
typedef uint list_index;
typedef complex<double> cxd;
typedef cxd* Gate;
typedef pair<Gate, vector<QID>> ApplicableGate;

using namespace std;

class QuantumLanguageInterpreter {
private:
  string lookUpFolder;
  ifstream file;
  QRegister *comp;
  unordered_map<string, vector<string>> userDefinedGates;
  unordered_map<string, unordered_map<string, string>> userDefinedValues;
  vector<ApplicableGate> gates;
  vector<QID> qubitsToReturn;
  string gateBeingExecuted;
  
  Gate gateFor(string g);
  
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
