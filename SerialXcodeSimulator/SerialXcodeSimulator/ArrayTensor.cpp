//
//  ArrayTensor.cpp
//  SerialXcodeSimulator
//
//  Created by Youssef Moawad on 15/11/2018.
//  Copyright © 2018 Youssef Moawad. All rights reserved.
//

#include "ArrayTensor.hpp"

ArrayTensor::ArrayTensor(int rows, int cols) : rows{rows}, cols{cols} {
  elements = new cxd[rows*cols];
  #pragma omp parallel for
  for(int i = 0; i < rows*cols; i++)
    elements[i] = 0;
}

ArrayTensor::ArrayTensor(cxd elts[], int rows, int cols) : rows{rows}, cols{cols} {
  assert(sizeof(cxd)*rows*cols/sizeof(*elts) == rows*cols);
  
  elements = new cxd[rows*cols];
//  memcpy(elements, elts, sizeof(cxd)*rows*cols);
  #pragma omp parallel for
  for(int i = 0; i < rows*cols; i++)
    elements[i] = elts[i];
}

int ArrayTensor::rowCount() {
  return rows;
}
int ArrayTensor::colCount() {
  return cols;
}

cxd ArrayTensor::elementAt(int r, int c) {
  return elements[cols*r+c];
}
void ArrayTensor::setElementAt(int r, int c, cxd newValue) {
  elements[cols*r+c] = newValue;
}

ArrayTensor ArrayTensor::getBlock(int rs, int re, int cs, int ce) {
  assert(re > rs);
  assert(ce > cs);
  ArrayTensor result (re-rs, ce-cs);
  
  #pragma omp parallel for
  for(int i = rs; i < re; i++) {
    #pragma omp parallel for
    for(int j = cs; j < ce; j++)
      result.setElementAt(i-rs, j-cs, elementAt(i, j));
  }
  
  return result;
}
void ArrayTensor::setBlock(int rs, int re, int cs, int ce, ArrayTensor t) {
  assert(re > rs);
  assert(ce > cs);
  assert(t.rowCount() == re-rs);
  assert(t.colCount() == ce-cs);
  
  #pragma omp parallel for
  for(int i = rs; i < re; i++)
    #pragma omp parallel for
    for(int j = cs; j < ce; j++)
      setElementAt(i, j, t.elementAt(i-rs, j-cs));
}

ArrayTensor ArrayTensor::multiplyBy(cxd k) {
  ArrayTensor result (rowCount(), colCount());
  
  #pragma omp parallel for
  for(int i = 0; i < rowCount(); i++)
    #pragma omp parallel for
    for(int j = 0; j < colCount(); j++)
      result.setElementAt(i, j, elementAt(i,j) * k);
  
  return result;
}


ArrayTensor ArrayTensor::kronWith(ArrayTensor t) {
  int t1r = rowCount();
  int t1c = colCount();
  int t2r = t.rowCount();
  int t2c = t.colCount();
  
  ArrayTensor result (t1r*t2r, t1c*t2c);
  
  #pragma omp parallel for
  for(int i = 0; i < t1r; i++)
    #pragma omp parallel for
    for(int j = 0; j < t1c; j++)
      result.setBlock(i*t2r, (i+1)*t2r, j*t2c, (j+1)*t2c, t.multiplyBy(elementAt(i, j)));
  
  return result;
}
