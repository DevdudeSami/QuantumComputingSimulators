//
//  SparseTensor.hpp
//  MatrixBasedSimulator
//
//  Created by Youssef Moawad on 29/11/2018.
//  Copyright © 2018 Youssef Moawad. All rights reserved.
//

#ifndef SparseTensor_hpp
#define SparseTensor_hpp

#include <stdio.h>
#include <complex>
#include <tuple>

#include "DenseTensor.hpp"
#include "Tensor.hpp"

using namespace std;

typedef pair<int, int> key;
typedef complex<double> cxd;

class DenseTensor;

class SparseTensor: public Tensor {
  
private:
  unsigned int r;
  unsigned int c;
  unsigned int nnz;
  key* keys;
  cxd* vals;
public:
  SparseTensor(unsigned int r, unsigned int c, unsigned int nnz, key* keys, cxd* vals);
  SparseTensor(DenseTensor t, unsigned int nnz);
  SparseTensor(vector<key> positions, unsigned int r, unsigned int c);
  
  unsigned int rowCount();
  unsigned int colCount();
  uint getNNZ();
  
  SparseTensor addTo(SparseTensor t);
  SparseTensor multiplyTo(cxd s);
  SparseTensor multiplyTo(SparseTensor t);
  SparseTensor multiplyToVector(SparseTensor v);
  SparseTensor sparseKronWith(SparseTensor t);
  SparseTensor sparseKronWith(DenseTensor t);
  SparseTensor sparseKronWith(Tensor* t);
  DenseTensor denseKronWith(SparseTensor t);
  DenseTensor denseKronWith(DenseTensor t);
  DenseTensor denseKronWith(Tensor* t);
  cxd dotProductWith(SparseTensor t);
  
  SparseTensor transpose();
  
  bool matchesDimensionsWith(SparseTensor t);
  bool isNormalised();
  
  cxd elementAt(unsigned int i, unsigned int j);
  bool elementIsNonZero(unsigned int i, unsigned int j);
  
  void enumerateElements(function<void(int,int,cxd)> f);
  void enumerateNNZElements(function<void(int,int,cxd)> f);

  DenseTensor dense();
  
  string toString();
};


  



#endif /* SparseTensor_hpp */
