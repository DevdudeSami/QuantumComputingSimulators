//
//  QuantumLanguageInterpreter.cpp
//  MatrixBasedSimulator
//
//  Created by Youssef Moawad on 18/01/2019.
//  Copyright © 2019 Youssef Moawad. All rights reserved.
//

#include "QuantumLanguageInterpreter.hpp"

using namespace std;
using namespace boost;

QuantumLanguageInterpreter::QuantumLanguageInterpreter(string lookUpFolder, string filename) : lookUpFolder{lookUpFolder} {
  
  file = ifstream(lookUpFolder + filename);
  comp = nullptr;
  gateBeingExecuted = "GLOBAL";
  
  if(!file) {
    cerr << "Unable to open file " << filename << endl;
    exit(1);
  }
}

Tensor* QuantumLanguageInterpreter::gateFor(string g) {
  if(g == "I") return new SparseTensor(IGate());
  else if(g == "X") return new SparseTensor(XGate());
  else if(g == "Y") return new SparseTensor(YGate());
  else if(g == "Z") return new SparseTensor(ZGate());
  else if(g == "H") return new SparseTensor(HGate());
  else if(g == "P") return new SparseTensor(PGate());
  else if(g == "CNOT") return new SparseTensor(CNOTGate());
  else if(g == "TOFF") return new SparseTensor(TOFFGate());
  else if(g == "SWAP") return new SparseTensor(SWAPGate());
  
  cerr << "Unrecognised gate: " << g << endl;
  exit(1);
}
    
string QuantumLanguageInterpreter::evalFromDefinedValues(string gate, string expr) {
  for(auto entry : userDefinedValues[gate]) {
    replace_all(expr, entry.first, entry.second);
  }
  return to_string((int) te_interp(expr.c_str(), 0));
}

void QuantumLanguageInterpreter::handleLine(vector<string> splitString, vector<ApplicableGate> *gates, ifstream *file) {
  if(splitString[0] == "loop") {
    string start = evalFromDefinedValues(gateBeingExecuted, splitString[2]);
    string end = evalFromDefinedValues(gateBeingExecuted, splitString[3]);
    string step = splitString.size() > 4 ? evalFromDefinedValues(gateBeingExecuted, splitString[4]) : "1";
  
    handleLoop(splitString[1], stoi(start), stoi(end), stoi(step), file, gates, nullptr);
    return;
  }
  
  // Handle let
  if(splitString[0] == "let") {
    handleLet(gateBeingExecuted, splitString[1], splitString[2]);
    return;
  }
  
  vector<QID> qubitIDs;
  
  if(splitString[1] == "all") {
    qubitIDs = comp->allQubits();
  } else {
    for(int i = 1; i < splitString.size(); i++) {
      if(splitString[i] == "--") break;
      qubitIDs.push_back(stoi(evalFromDefinedValues(gateBeingExecuted, splitString[i])));
    }
  }
  
  // Handle user defined gate being called
  if(userDefinedGates.find(splitString[0]) != userDefinedGates.end()) {
    vector<string> linesForUserDefinedGate = userDefinedGates[splitString[0]];
    userDefinedValues[splitString[0]]["argc"] = to_string(qubitIDs.size());
    
    gateBeingExecuted = splitString[0];
    
    for(int i = 0; i < linesForUserDefinedGate.size(); i++) {
      string line = linesForUserDefinedGate[i];
      
      vector<string> splitString;
      split(splitString, line, is_any_of(" "));
      
      // Handle loop in a user defined gate
      if(splitString[0] == "loop") {
        vector<string> loopLines;
        i++;
        while(true) {
          line = linesForUserDefinedGate[i];
          if(line == "endloop") break;
          loopLines.push_back(line);
          i++;
        }
        
        string start = evalFromDefinedValues(gateBeingExecuted, splitString[2]);
        string end = evalFromDefinedValues(gateBeingExecuted, splitString[3]);
        string step = splitString.size() > 4 ? evalFromDefinedValues(gateBeingExecuted, splitString[4]) : "1";
        
        handleLoop(splitString[1], stoi(start), stoi(end), stoi(step), file, gates, &loopLines);
        continue;
      }
      
      // Handle other lines
      handleLine(splitString, gates, file);
    }
    
    gateBeingExecuted = "GLOBAL";
    
  } else gates->push_back(ApplicableGate(gateFor(splitString[0]), qubitIDs));
}

void QuantumLanguageInterpreter::handleLink(ifstream *file) {
  string line;
  vector<string> splitString;
  
  while(getline(*file, line)) {
    trim(line);
    if(line.empty()) continue;
    split(splitString, line, is_any_of(" "));
    if(splitString[0] == "--") continue;
    
    // Handle link
    if(splitString[0] == "link" || splitString[0] == "import") {
      ifstream linkedFile (lookUpFolder + splitString[1] + ".qll");
      if(!linkedFile) {
        cerr << "Unable to open file " << splitString[1] << ".qll" << endl;
        exit(1);
      }

      handleLink(&linkedFile);
      continue;
    }
    
    // Handle self defined gates
    if(splitString[0] == "gate") {
      selfDefineGate(splitString[1], file);
      continue;
    }
  }
}
    
void QuantumLanguageInterpreter::handleLoop(string symbol, int start, int end, int step, ifstream *file, vector<ApplicableGate> *gates, vector<string> *lines) {
  string line;
  vector<string> splitString;
  vector<string> loopLines;
  
  if(lines != nullptr)
    loopLines = *lines;
  else {
    while(getline(*file, line)) {
      trim(line);
      if(line.empty()) continue;
      if(line == "endloop")
        break;
      loopLines.push_back(line);
    }
  }
  
  for(int n = start; step >= 0 ? n <= end : n >= end; n += step) {
    for(string line : loopLines) {
      split(splitString, line, is_any_of(" "));
      
      for(int i = 1; i < splitString.size(); i++) {
        replace_all(splitString[i], symbol, to_string(n));
        splitString[i] = to_string((int) te_interp(splitString[i].c_str(), 0));
      }
      
      handleLine(splitString, gates, file);
    }
  }
}
    
void QuantumLanguageInterpreter::handleLet(string gate, string symbol, string expr) {
  
  userDefinedValues[gate][symbol] = evalFromDefinedValues(gate, expr);
}
    
void QuantumLanguageInterpreter::selfDefineGate(string gateName, ifstream *file) {
  
  string line;
  vector<string> splitString;
  
  vector<string> linesForGateBeingDefined;
  
  while(getline(*file, line)) {
    trim(line);
    if(line.empty()) continue;
    split(splitString, line, is_any_of(" "));
    if(splitString[0] == "--") continue;
    if(splitString[0] == "endgate") break;
    
    linesForGateBeingDefined.push_back(line);
  }
  
  userDefinedGates[gateName] = linesForGateBeingDefined;
}


string QuantumLanguageInterpreter::execute() {
  int numberOfMeasurements = 0;
  
  string line;
  vector<string> splitString;

  while(getline(file, line)) {
    if(line.empty()) continue;
    trim(line);
    split(splitString, line, is_any_of(" "));
    if(splitString[0] == "--") continue;
    
    // Handle init step
    if(splitString[0] == "init") {
      comp = new QComputer(stoi(splitString[1]));
      continue;
    }
    
    // Handle self defined gates
    if(splitString[0] == "gate") {
      selfDefineGate(splitString[1], &file);
      continue;
    }
    
    // Handle return
    if(splitString[0] == "return") {
      if(splitString[1] == "all") {
        qubitsToReturn = comp->allQubits();
      }
      else for(int i = 1; i < splitString.size(); i++) qubitsToReturn.push_back(stoi(splitString[i]));
      continue;
    }
    
    // Handle take
    if(splitString[0] == "take") {
      numberOfMeasurements = stoi(splitString[1]);
      continue;
    }
    
    // Handle link
    if(splitString[0] == "link" || splitString[0] == "import") {
      ifstream linkedFile (lookUpFolder + splitString[1] + ".qll");
      if(!linkedFile) {
        cerr << "Unable to open file " << splitString[1] << ".qll" << endl;
        exit(1);
      }
      
      handleLink(&linkedFile);
      continue;
    }
    
    // Handle any other line
    handleLine(splitString, &gates, &file);
  }
  
  file.close();
  
  CircuitOptimiser co (comp, gates);
  co.executeCircuit();
  
  if(numberOfMeasurements != 0) {
    cout << "Took " << numberOfMeasurements << " measurements: " << takeMeasurementsInString(*comp, numberOfMeasurements, nullptr) << endl;
  }
  
  string m = comp->measure();
  string result;
  
  for(QID q : qubitsToReturn) {
    result += m[q+1];
  }
  
  return result;
}
