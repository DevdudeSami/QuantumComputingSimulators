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
#include <map>
#include <unordered_map>
#include "Tensor.hpp"

using namespace std;

typedef pair<int, int> key;
typedef map<key, cxd> dok;
//typedef complex<double> cxd;

class SparseTensor {
  
private:
  unsigned int r;
  unsigned int c;
  unsigned int nnz;
//  dok d;
  key* keys;
  cxd* vals;
//  static vector<key> keys(dok const& m);
public:
  SparseTensor(unsigned int r, unsigned int c, unsigned int nnz, key* keys, cxd* vals);
  SparseTensor(Tensor t, unsigned int nnz);
  SparseTensor(vector<key> positions, unsigned int r, unsigned int c);
  
  SparseTensor addTo(SparseTensor m);
//  SparseTensor kMultiplyTo(cxd s);
//  SparseTensor multiplyTo(SparseTensor m);
  SparseTensor kronWith(SparseTensor m);
  
  cxd* elementAt(int r, int c);
//  void setElementAt(int r, int c, cxd e);
  
  Tensor dense();
  
};


  



#endif /* SparseTensor_hpp */
