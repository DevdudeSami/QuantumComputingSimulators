//
//  Tensor.hpp
//  SerialXcodeSimulator
//
//  Created by Youssef Moawad on 09/11/2018.
//  Copyright © 2018 Youssef Moawad. All rights reserved.
//

#ifndef Tensor_hpp
#define Tensor_hpp


#include <iostream>
#include <complex>
#include <vector>

using namespace std;

using cxd = complex<double>;
using Row = vector<cxd>;
using Rows = vector<Row>;

class Tensor {
private:
  Rows rows;
public:
  Tensor(Rows rs);
  Tensor(int rowCount, int colCount);
  Tensor(vector<cxd> v);
  int rowCount();
  int colCount();
  cxd elementAt(int r, int c);
  void setElementAt(int r, int c, cxd newValue);
  Row getRow(int r);
  Row getCol(int c);
  Rows getRows();
  bool matchesDimensionsWith(Tensor t);
  Tensor getBlock(int rs, int re, int cs, int ce);
  void setBlock(int rs, int re, int cs, int ce, Tensor t);
  void enumerateElements(function<void(int,int,cxd)> f);
  bool isNormalised();
  Tensor transpose();
  
  cxd dotProductWith(Tensor t);
  Tensor multiplyBy(cxd k);
  Tensor addTo(Tensor t);
  
  Tensor multiplyBy(Tensor t);
  
  Tensor kronWith(Tensor t);
  
  std::string toString();
  
  /** Static Functions **/
  static cxd vectorDotProduct(Row v1, Row v2);
};

#endif /* Tensor_hpp */
