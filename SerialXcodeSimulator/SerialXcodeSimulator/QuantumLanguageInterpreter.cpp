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

QuantumLanguageInterpreter::QuantumLanguageInterpreter(string lookUpFolder, string filename) : lookUpFolder{lookUpFolder}, filename{filename} {}

unordered_map<string, vector<ApplicableGate>> userDefinedGates;
vector<ApplicableGate> gates;
vector<QID> qubitsToReturn;

Tensor *gateFor(string g) {
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

void handleLine(QComputer *comp, vector<string> splitString, vector<ApplicableGate> *gates) {
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

void selfDefineGate(QComputer *comp, string gateName, ifstream *file) {
  string line;
  vector<string> splitString;
  
  vector<ApplicableGate> gatesForGateBeingDefined;
  
  while(getline(*file, line)) {
    trim(line);
    if(line.empty()) continue;
    split(splitString, line, is_any_of(" "));
    if(splitString[0] == "--") continue;
    if(splitString[0] == "endgate") break;
    
    handleLine(comp, splitString, &gatesForGateBeingDefined);
  }
  
  userDefinedGates[gateName] = gatesForGateBeingDefined;
}

void QuantumLanguageInterpreter::handleImport(QComputer *comp, ifstream *file) {
  string line;
  vector<string> splitString;
  
  while(getline(*file, line)) {
    trim(line);
    if(line.empty()) continue;
    split(splitString, line, is_any_of(" "));
    if(splitString[0] == "--") continue;
    
    // Handle link
    if(splitString[0] == "link") {
      ifstream linkedFile (lookUpFolder + splitString[1] + ".qll");
      if(!linkedFile) {
        cerr << "Unable to open file " << splitString[1] << ".qll" << endl;
        exit(1);
      }
      
      handleImport(comp, &linkedFile);
      continue;
    }
    
    if(splitString[0] == "gate") {
      selfDefineGate(comp, splitString[1], file);
      continue;
    }
  }
}

string QuantumLanguageInterpreter::execute() {
  QComputer *comp = nullptr;
  
  int numberOfMeasurements = 0;
  
  ifstream file (lookUpFolder + filename);
  
  if(!file) {
    cerr << "Unable to open file " << filename << endl;
    exit(1);
  }
  
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
      selfDefineGate(comp, splitString[1], &file);
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
    if(splitString[0] == "link") {
      ifstream linkedFile (lookUpFolder + splitString[1] + ".qll");
      if(!linkedFile) {
        cerr << "Unable to open file " << splitString[1] << ".qll" << endl;
        exit(1);
      }
      
      handleImport(comp, &linkedFile);
      continue;
    }
    
    // Handle any other line
    handleLine(comp, splitString, &gates);
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
