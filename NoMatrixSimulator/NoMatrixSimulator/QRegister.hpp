//
//  QRegister.hpp
//  NoMatrixSimulator
//
//  Created by Youssef Moawad on 05/02/2019.
//  Copyright © 2019 Youssef Moawad. All rights reserved.
//

#ifndef QRegister_hpp
#define QRegister_hpp

#include <stdio.h>
#include <complex>
#include <vector>
#include <random>
#include <omp.h>

using namespace std;

typedef unsigned int QID;
typedef unsigned int list_index;
typedef complex<double> cxd;

class QRegister {
  
private:
  uint n;
  cxd* amplitudes;
  
public:
  QRegister(uint n);
  
  void applySingleGate(QID qID, cxd gate[4]);
  void applyCnGate(vector<QID> controls, QID qID, cxd gate[4]);

  string measure();
  
};


#endif /* QRegister_hpp */
