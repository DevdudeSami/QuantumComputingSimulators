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
#include "Adders.hpp"
#include "Algorithms.hpp"

using namespace std;

int main(int argc, const char * argv[]) {
  auto start = chrono::high_resolution_clock::now();

  auto end = chrono::high_resolution_clock::now();
  auto diff = end - start;
  cout << "--- end time: " << (chrono::duration <double, milli> (diff).count())/1000 << " s" << endl;
  
  return 0;
}
