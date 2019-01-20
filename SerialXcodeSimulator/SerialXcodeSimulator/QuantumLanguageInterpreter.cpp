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

QuantumLanguageInterpreter::QuantumLanguageInterpreter(string filename) : filename{filename} {}

unordered_map<string, vector<ApplicableGate>> userDefinedGates;

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
  
  cerr << "Unrecognised gate." << endl;
  exit(1);
}

string QuantumLanguageInterpreter::execute() {
  QComputer *comp;
  vector<ApplicableGate> gates;
  vector<QID> qubitsToReturn;
  
  ifstream file (filename);
  
  if(!file) {
    cerr << "Unable to open file " << filename << endl;
    exit(1);   // call system to stop
  }
  
  string line;
  vector<string> splitString;

  while(getline(file, line)) {
    if(line.empty()) continue;
    split(splitString, line, is_any_of(" "));
    if(splitString[0] == "--") continue;
    
    // init step
    if(splitString[0] == "init") {
      comp = new QComputer(stoi(splitString[1]));
      continue;
    }
    
    // SELF DEFINED GATES
    if(splitString[0] == "gate") {
      string gateBeingDefined = splitString[1];
      vector<ApplicableGate> gatesForGateBeingDefined;
      
      while(getline(file, line)) {
        split(splitString, line, is_any_of(" "));
        if(splitString[0] == "endgate") break;
        
        vector<QID> qubitIDs;
        
        if(splitString[1] == "all") {
          qubitIDs = comp->allQubits();
        } else {
          for(int i = 1; i < splitString.size(); i++) {
            if(splitString[i] == "--") break;
            qubitIDs.push_back(stoi(splitString[i]));
          }
        }
        
        gatesForGateBeingDefined.push_back(ApplicableGate(gateFor(splitString[0]), qubitIDs));
      }
      
      userDefinedGates[gateBeingDefined] = gatesForGateBeingDefined;
      continue;
    }
    
    if(splitString[0] != "return") {
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
          
          gates.push_back(ApplicableGate(g.first, qubitIDsPrime));
        }
        
      } else gates.push_back(ApplicableGate(gateFor(splitString[0]), qubitIDs));
      
    } else {
      if(splitString[1] == "all") {
        qubitsToReturn = comp->allQubits();
      }
      else for(int i = 1; i < splitString.size(); i++) qubitsToReturn.push_back(stoi(splitString[i]));
    }
  }
  
  file.close();
  
  CircuitOptimiser co (comp, gates);
  co.executeCircuit();
  
  string m = comp->measure();
  string result;
  
  for(QID q : qubitsToReturn) {
    result += m[q+1];
  }
  
  return result;
}
