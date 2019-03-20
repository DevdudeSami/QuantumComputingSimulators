//
//  main.cpp
//  SerialXcodeSimulator
//
//  Created by Youssef Moawad on 02/11/2018.
//  Copyright © 2018 Youssef Moawad. All rights reserved.
//

#include <iostream>
#include <time.h>

#include "Adders.hpp"
#include "QuantumErrorCorrection.hpp"

using namespace std;

int main(int argc, char const *argv[]) {
  auto start = chrono::high_resolution_clock::now();

  CircuitOptimisedNQubitCuccaroAdder(6, "111111", "111111");

  auto end = chrono::high_resolution_clock::now();
  auto diff = end - start;
  // cout << "--- end time: " << (chrono::duration <double, milli> (diff).count())/1000 << " s" << endl;
  cout << (chrono::duration <double, milli> (diff).count())/1000;

  return 0;
}
