//
//  Gates.cpp
//  SerialXcodeSimulator
//
//  Created by Youssef Moawad on 09/11/2018.
//  Copyright © 2018 Youssef Moawad. All rights reserved.
//

#include "Gates.hpp"

/************* Q Gates ***************/

SparseTensor I() {
  key keys[2] = {make_pair(0, 0), make_pair(1, 1)};
  cxd vals[2] = {cxd(1), cxd(1)};
  unsigned int r = 2;
  unsigned int c = 2;
  unsigned int nnz = 2;
  
  return SparseTensor(r, c, nnz, keys, vals);
}

SparseTensor X() {
  key keys[2] = {make_pair(0, 1), make_pair(1, 0)};
  cxd vals[2] = {cxd(1), cxd(1)};
  unsigned int r = 2;
  unsigned int c = 2;
  unsigned int nnz = 2;
  
  return SparseTensor(r, c, nnz, keys, vals);
}

SparseTensor Y() {
  key keys[2] = {make_pair(0, 0), make_pair(1, 1)};
  cxd vals[2] = {cxd(0,1), cxd(0,-1)};
  unsigned int r = 2;
  unsigned int c = 2;
  unsigned int nnz = 2;
  
  return SparseTensor(r, c, nnz, keys, vals);
}

SparseTensor Z() {
  key keys[2] = {make_pair(0, 0), make_pair(1, 1)};
  cxd vals[2] = {cxd(1), cxd(-1)};
  unsigned int r = 2;
  unsigned int c = 2;
  unsigned int nnz = 2;
  
  return SparseTensor(r, c, nnz, keys, vals);
}

SparseTensor P() {
  key keys[2] = {make_pair(0, 0), make_pair(1, 1)};
  cxd vals[2] = {cxd(1), cxd(0,1)};
  unsigned int r = 2;
  unsigned int c = 2;
  unsigned int nnz = 2;
  
  return SparseTensor(r, c, nnz, keys, vals);
}

SparseTensor H() {
  key keys[4] = {make_pair(0, 0), make_pair(0, 1), make_pair(1, 0), make_pair(1, 1)};
  cxd vals[4] = {cxd(1), cxd(1), cxd(1), cxd(-1)};
  unsigned int r = 2;
  unsigned int c = 2;
  unsigned int nnz = 4;
  
  SparseTensor t (r, c, nnz, keys, vals);
  
  return t.multiplyTo(1/sqrt(2));
}

SparseTensor CNOT() {
  key keys[4] = {make_pair(0, 0), make_pair(1, 1), make_pair(2, 3), make_pair(3, 2)};
  cxd vals[4] = {cxd(1), cxd(1), cxd(1), cxd(1)};
  unsigned int r = 4;
  unsigned int c = 4;
  unsigned int nnz = 4;
  
  return SparseTensor(r, c, nnz, keys, vals);
}

SparseTensor SWAP() {
  key keys[4] = {make_pair(0, 0), make_pair(1, 2), make_pair(2, 1), make_pair(3, 3)};
  cxd vals[4] = {cxd(1), cxd(1), cxd(1), cxd(1)};
  unsigned int r = 4;
  unsigned int c = 4;
  unsigned int nnz = 4;

  return SparseTensor(r, c, nnz, keys, vals);
}

SparseTensor TOFF() {
  key keys[8] = {make_pair(0, 0), make_pair(1, 1), make_pair(2, 2), make_pair(3, 3), make_pair(4, 4), make_pair(5, 5), make_pair(6, 7), make_pair(7, 6)};
  cxd vals[8] = {cxd(1), cxd(1), cxd(1), cxd(1), cxd(1), cxd(1), cxd(1), cxd(1)};
  unsigned int r = 8;
  unsigned int c = 8;
  unsigned int nnz = 8;
  
  return SparseTensor(r, c, nnz, keys, vals);
}

/************* END Q Gates ***************/
