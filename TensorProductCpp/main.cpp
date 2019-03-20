//
//  main.cpp
//  SerialXcodeSimulator
//
//  Created by Youssef Moawad on 02/11/2018.
//  Copyright © 2018 Youssef Moawad. All rights reserved.
//

#include <iostream>
#include <time.h>

#include "DenseTensor.hpp"
#include "Gates.hpp"
#include "StateVector.hpp"
#include "SparseTensor.hpp"
#include "Accumulator.hpp"
#include "QComputer.hpp"
#include "Algorithms.hpp"
#include "Adders.hpp"

using namespace std;

int main(int argc, char const *argv[]) {
  auto start = chrono::high_resolution_clock::now();
  
  AltSixQubitRippleCarryAdder("111111", "111111");

  auto end = chrono::high_resolution_clock::now();
  auto diff = end - start;
  cout << (chrono::duration <double, milli> (diff).count())/1000;

  return 0;
}