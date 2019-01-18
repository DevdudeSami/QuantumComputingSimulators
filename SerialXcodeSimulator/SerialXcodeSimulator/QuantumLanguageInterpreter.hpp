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
#include <boost/algorithm/string.hpp>

using namespace std;

class QuantumLanguageInterpreter {
private:
  string filename;
public:
  QuantumLanguageInterpreter(string filename);
  string execute();
};


#endif /* QuantumLanguageInterpreter_hpp */
