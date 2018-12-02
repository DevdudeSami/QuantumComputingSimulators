//
//  SparseTensor.hpp
//  SerialXcodeSimulator
//
//  Created by Youssef Moawad on 29/11/2018.
//  Copyright © 2018 Youssef Moawad. All rights reserved.
//

#ifndef SparseTensor_hpp
#define SparseTensor_hpp

#include <stdio.h>
#include <complex>
#include <tuple>
#include "Tensor.hpp"

using namespace std;

typedef pair<int, int> key;

class SparseTensor {
  
private:
  unsigned int r;
  unsigned int c;
  unsigned int nnz;
  key* keys;
  cxd* vals;
public:
  SparseTensor(unsigned int r, unsigned int c, unsigned int nnz, key* keys, cxd* vals);
  SparseTensor(Tensor t, unsigned int nnz);
  SparseTensor(vector<key> positions, unsigned int r, unsigned int c);
  
  SparseTensor addTo(SparseTensor m);
  SparseTensor kMultiplyTo(cxd s);
  SparseTensor multiplyTo(SparseTensor m);
  SparseTensor kronWith(SparseTensor m);
  
  SparseTensor transpose();
  
  bool matchesDimensionsWith(SparseTensor t);
  bool isNormalised();
  
  cxd* elementAt(int r, int c);
  bool elementIsNonZero(unsigned int i, unsigned int j);
  
  void enumerateElements(function<void(int,int,cxd)> f);
  void enumerateNNZElements(function<void(int,int,cxd)> f);

  Tensor dense();
  
};


  



#endif /* SparseTensor_hpp */
