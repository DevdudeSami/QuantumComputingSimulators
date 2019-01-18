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

  // init step
  getline(file, line);
  vector<string> splitString;
  split(splitString, line, is_any_of(" "));
  if(splitString[0] != "init") {
    cerr << "The first line isn't an init statement." << endl;
    exit(1);
  }
  comp = new QComputer(stoi(splitString[1]));
  
  while(getline(file, line)) {
    split(splitString, line, is_any_of(" "));
    if(splitString[0] != "return") {
      vector<QID> qubitIDs;
      for(int i = 1; i < splitString.size(); i++) qubitIDs.push_back(stoi(splitString[i]));
      
      gates.push_back(ApplicableGate(gateFor(splitString[0]), qubitIDs));
    } else {
      for(int i = 1; i < splitString.size(); i++) qubitsToReturn.push_back(stoi(splitString[i]));
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
