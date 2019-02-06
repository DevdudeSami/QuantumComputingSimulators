//
//  main.cpp
//  NoMatrixSimulator
//
//  Created by Youssef Moawad on 05/02/2019.
//  Copyright © 2019 Youssef Moawad. All rights reserved.
//

#include <iostream>
#include "QRegister.hpp"
#include "Gates.hpp"
#include "Accumulator.hpp"

using namespace std;

int main(int argc, const char * argv[]) {
  auto start = chrono::high_resolution_clock::now();

  QRegister reg (20);
  
  reg.applySingleGate(7, H);
  reg.applyCnGate({7}, 17, X);

//  cout << takeMeasurementsInString(reg, 100) << endl;
  
  auto end = chrono::high_resolution_clock::now();
  auto diff = end - start;
  cout << "--- end time: " << (chrono::duration <double, milli> (diff).count())/1000 << " s" << endl;
  
  return 0;
}
