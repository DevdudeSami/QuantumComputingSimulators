//
//  SparseTensor.cpp
//  SerialXcodeSimulator
//
//  Created by Youssef Moawad on 29/11/2018.
//  Copyright © 2018 Youssef Moawad. All rights reserved.
//

#include "SparseTensor.hpp"
#include <omp.h>

SparseTensor::SparseTensor(unsigned int r, unsigned int c, unsigned int nnz, key* keys, cxd* vals) : r{r},  c{c}, nnz{nnz}, keys{keys}, vals{vals} {}

SparseTensor::SparseTensor(Tensor t, unsigned int nnz) : nnz{nnz} {
  r = t.rowCount();
  c = t.colCount();
  
  vals = new cxd[nnz];
  keys = new key[nnz];
  
  int i = 0;
  
  t.enumerateElements([this, &i](int r, int c, cxd v) {
    if(v != 0.0) {
      
      vals[i] = v;
      keys[i] = make_pair(r, c);
      
      i++;
    }
  });
}

// TODO: Fix this! This will crash if used.
SparseTensor::SparseTensor(vector<key> positions, unsigned int r, unsigned int c) : r{r}, c{c} {

  nnz = positions.size();
  vector<cxd> values (positions.size(), 1);
  
  vals = &values[0];
  keys = &positions[0];
}

SparseTensor SparseTensor::addTo(SparseTensor m) {
  assert(r == m.r);
  assert(c == m.c);
  
  vector<cxd> values;
  vector<key> ks;
  unsigned int new_nnz = 0;

  #pragma omp for
  for(int i = 0; i < nnz; i++) {
    if(find(m.keys, m.keys + m.nnz, keys[i]) != m.keys + m.nnz) {
      ks.push_back(keys[i]);
      values.push_back(vals[i]+m.vals[i]);
      new_nnz++;
    }
    else {
      ks.push_back(keys[i]);
      values.push_back(vals[i]);
      new_nnz++;
    }
  }
  
  #pragma omp for
  for(int i = 0; i < m.nnz; i++) {
    if(find(keys, keys+nnz, m.keys[i]) == keys+nnz) {
      ks.push_back(m.keys[i]);
      values.push_back(m.vals[i]);
      new_nnz++;
    }
  }
  
  cxd *new_vals = new cxd[new_nnz];
  key *new_keys = new key[new_nnz];
  
  copy(values.begin(), values.end(), new_vals);
  copy(ks.begin(), ks.end(), new_keys);
  
  return SparseTensor(r,c,nnz,new_keys,new_vals);
}

//SparseTensor SparseTensor::kMultiplyTo(cxd s) {
//  dok d_;
//
//  for(auto const& k : keys(d)) d_[k] = s*d[k];
//
//  return SparseTensor(r,c,d_);
//}
//
//SparseTensor SparseTensor::multiplyTo(SparseTensor m) {
//  assert(c == m.r);
//
//  dok d_;
//
//  vector<key> keys1 = keys(d);
//  vector<key> keys2 = keys(m.d);
//
//  for(int i = 0; i < keys1.size(); i++) {
//    for(int j = 0; j < keys2.size(); j++) {
//      key k1 = keys1[i];
//      key k2 = keys2[j];
//      if(get<1>(k1) == get<0>(k2)) {
//        vector<key> d_keys = keys(d_);
//        key k = make_pair(get<0>(k1), get<1>(k2));
//
//        if(find(d_keys.begin(), d_keys.end(), k) != d_keys.end()) d_[k] += d[k1]*m.d[k2];
//        else d_[k] = d[k1]*m.d[k2];
//      }
//    }
//  }
//
//  return SparseTensor(r,m.c,d_);
//}
//
SparseTensor SparseTensor::kronWith(SparseTensor m) {

  key* new_keys = new key[nnz*m.nnz];
  cxd* new_vals = new cxd[nnz*m.nnz];

  #pragma omp parallel for
  for(int i = 0; i < nnz; i++) {
    #pragma omp parallel for
    for(int j = 0; j < m.nnz; j++) {
      key k1 = keys[i];
      key k2 = m.keys[j];

      new_keys[i*m.nnz+j] = make_pair(k1.first*m.r+k2.first, k1.second*m.c+k2.second);
      new_vals[i*m.nnz+j] = vals[i] * m.vals[j];
    }
  }

  return SparseTensor(r*m.r,c*m.c,nnz*m.nnz,new_keys,new_vals);
}

Tensor SparseTensor::dense() {
  Tensor t (r,c);
  
  for(int i = 0; i < nnz; i++) {
    key k = keys[i];
    t.setElementAt(k.first, k.second, vals[i]);
  }
  
  return t;
}

cxd *SparseTensor::elementAt(int r, int c) {
  key k = make_pair(r, c);

  for(int i = 0; i < nnz; i++) {
    if(keys[i] == k) return &vals[i];
  }
  
  return nullptr;
}

//vector<key> SparseTensor::keys(dok const& m) {
//  vector<key> result;
//  for (auto const& i : m) {
//    result.push_back(i.first);
//  }
//  return result;
//}
