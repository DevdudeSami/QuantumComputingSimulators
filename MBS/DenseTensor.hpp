//
//  DenseTensor.hpp
//  MatrixBasedSimulator
//
//  Created by Youssef Moawad on 09/11/2018.
//  Copyright © 2018 Youssef Moawad. All rights reserved.
//

#ifndef DenseTensor_hpp
#define DenseTensor_hpp


#include <iostream>
#include <complex>
#include <vector>

#include "SparseTensor.hpp"
#include "Tensor.hpp"

using namespace std;

using cxd = complex<double>;
using Row = vector<cxd>;
using Rows = vector<Row>;

class SparseTensor;

class DenseTensor: public Tensor {
private:
  Rows rows;
public:
  DenseTensor(Rows rs);
  DenseTensor(int rowCount, int colCount);
  DenseTensor(vector<cxd> v);
  uint rowCount();
  uint colCount();
  uint getNNZ();
  cxd elementAt(int r, int c);
  void setElementAt(int r, int c, cxd newValue);
  Row getRow(int r);
  Row getCol(int c);
  Rows getRows();
  bool matchesDimensionsWith(DenseTensor t);
  DenseTensor getBlock(int rs, int re, int cs, int ce);
  void setBlock(int rs, int re, int cs, int ce, DenseTensor t);
  void enumerateElements(function<void(int,int,cxd)> f);
  bool isNormalised();
  DenseTensor transpose();
  
  cxd dotProductWith(DenseTensor t);
  DenseTensor multiplyBy(cxd k);
  DenseTensor addTo(DenseTensor t);
  
  DenseTensor multiplyTo(DenseTensor t);
  SparseTensor multiplyTo(SparseTensor t);
  
  SparseTensor sparseKronWith(SparseTensor t);
  SparseTensor sparseKronWith(DenseTensor t);
  SparseTensor sparseKronWith(Tensor* t);
  DenseTensor denseKronWith(SparseTensor t);
  DenseTensor denseKronWith(DenseTensor t);
  DenseTensor denseKronWith(Tensor* t);
  
  std::string toString();
  
  /** Static Functions **/
  static cxd vectorDotProduct(Row v1, Row v2);
};

#endif /* DenseTensor_hpp */
