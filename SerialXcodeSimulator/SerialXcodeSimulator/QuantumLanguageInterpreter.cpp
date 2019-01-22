//
//  QuantumLanguageInterpreter.cpp
//  SerialXcodeSimulator
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
  gateBeingDefined = "GLOBAL";
  
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
    string start = evalFromDefinedValues(gateBeingDefined, splitString[2]);
    string end = evalFromDefinedValues(gateBeingDefined, splitString[3]);
    
    handleLoop(splitString[1], stoi(start), stoi(end), file, gates);
    return;
  }
  
  // Handle let
  if(splitString[0] == "let") {
    handleLet(gateBeingDefined, splitString[1], splitString[2]);
    return;
  }
  
  for(int i = 1; i < splitString.size(); i++) {
    evalFromDefinedValues(gateBeingDefined, splitString[i]);
  }
  
  vector<QID> qubitIDs;
  
  if(splitString[1] == "all") {
    qubitIDs = comp->allQubits();
  } else {
    for(int i = 1; i < splitString.size(); i++) {
      if(splitString[i] == "--") break;
      qubitIDs.push_back(stoi(splitString[i]));
    }
  }
  
  if(userDefinedGates.find(splitString[0]) != userDefinedGates.end()) {
    vector<ApplicableGate> gatesForUserDefinedGate = userDefinedGates[splitString[0]];
    vector<ApplicableGate> finalGates;
    
    for(ApplicableGate g : gatesForUserDefinedGate) {
      vector<QID> qubitIDsPrime;
      
      for(QID q : g.second) {
        qubitIDsPrime.push_back(qubitIDs[q]);
      }
      
      gates->push_back(ApplicableGate(g.first, qubitIDsPrime));
    }
    
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
      selfDefineGate(splitString[1], splitString[2], file);
      continue;
    }
  }
}
    
void QuantumLanguageInterpreter::handleLoop(string symbol, int start, int end, ifstream *file, vector<ApplicableGate> *gates) {
  string line;
  vector<string> splitString;
  vector<string> loopLines;
  
  while(getline(*file, line)) {
    trim(line);
    if(line.empty()) continue;
    if(line == "endloop")
      break;
    loopLines.push_back(line);
  }
  
  for(int n = start; n <= end; n++) {
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
    
void QuantumLanguageInterpreter::selfDefineGate(string gateName, string argc, ifstream *file) {
  gateBeingDefined = gateName;
  userDefinedValues[gateName]["argc"] = argc;
  
  string line;
  vector<string> splitString;
  
  vector<ApplicableGate> gatesForGateBeingDefined;
  
  while(getline(*file, line)) {
    trim(line);
    if(line.empty()) continue;
    split(splitString, line, is_any_of(" "));
    if(splitString[0] == "--") continue;
    if(splitString[0] == "endgate") break;
    
    handleLine(splitString, &gatesForGateBeingDefined, file);
  }
  
  userDefinedGates[gateName] = gatesForGateBeingDefined;
  
  gateBeingDefined = "GLOBAL";
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
      selfDefineGate(splitString[1], splitString[2], &file);
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
