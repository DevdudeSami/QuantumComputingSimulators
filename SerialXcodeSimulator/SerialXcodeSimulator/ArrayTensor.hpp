//
//  ArrayTensor.hpp
//  SerialXcodeSimulator
///Users/youssefmoawad/Documents/University/Year 4/Project/SerialXcodeSimulator/SerialXcodeSimulator/main.cpp
//  Created by Youssef Moawad on 15/11/2018.
//  Copyright © 2018 Youssef Moawad. All rights reserved.
//

#ifndef ArrayTensor_hpp
#define ArrayTensor_hpp

#include <stdio.h>
#include <iostream>
#include <complex>
#include <omp.h>

using namespace std;

using cxd = complex<double>;

class ArrayTensor {
private:
  int rows;
  int cols;
  cxd* elements;
public:
//  ArrayTensor(cxd *rows[]);
  ArrayTensor(int rows, int cols);
  ArrayTensor(cxd elts[], int rows, int cols);
  int rowCount();
  int colCount();
  cxd elementAt(int r, int c);
  void setElementAt(int r, int c, cxd newValue);
  ArrayTensor getBlock(int rs, int re, int cs, int ce);
  void setBlock(int rs, int re, int cs, int ce, ArrayTensor t);
  ArrayTensor multiplyBy(cxd k);
  ArrayTensor kronWith(ArrayTensor t);
};


#endif /* ArrayTensor_hpp */
