//
//  SparseTensor.cpp
//  SerialXcodeSimulator
//
//  Created by Youssef Moawad on 29/11/2018.
//  Copyright © 2018 Youssef Moawad. All rights reserved.
//

#include "SparseTensor.hpp"
#include <omp.h>

SparseTensor::SparseTensor(unsigned int r, unsigned int c, dok d) : r{r},  c{c}, d{d} {}

SparseTensor::SparseTensor(Tensor t) {
  r = t.rowCount();
  c = t.colCount();
  dok d_;
  
  t.enumerateElements([&d_](int r, int c, cxd v) {
    d_[make_pair(r, c)] = v;
  });
  
  d = d_;
}

SparseTensor::SparseTensor(vector<key> positions, unsigned int r, unsigned int c) : r{r}, c{c} {
  
  dok d_;
  
  for(auto const& p: positions) d_[p] = cxd(1);
  
  d = d_;
}

SparseTensor SparseTensor::addTo(SparseTensor m) {
  assert(r == m.r);
  assert(c == m.c);
  
  dok d_;
  
  vector<key> keys1 = keys(d);
  vector<key> keys2 = keys(m.d);
  
  for(auto const& k : keys1) {
    if(find(keys2.begin(), keys2.end(), k) != keys2.end()) d_[k] = d[k] + m.d[k];
    else d_[k] = d[k];
  }
  
  for(auto const& k : keys2) {
    if(find(keys1.begin(), keys1.end(), k) == keys1.end()) d_[k] = m.d[k];
  }
  
  return SparseTensor(r,c,d_);
  
}

SparseTensor SparseTensor::kMultiplyTo(cxd s) {
  dok d_;
  
  for(auto const& k : keys(d)) d_[k] = s*d[k];
  
  return SparseTensor(r,c,d_);
}

SparseTensor SparseTensor::multiplyTo(SparseTensor m) {
  assert(c == m.r);

  dok d_;
  
  vector<key> keys1 = keys(d);
  vector<key> keys2 = keys(m.d);
  
  for(int i = 0; i < keys1.size(); i++) {
    for(int j = 0; j < keys2.size(); j++) {
      key k1 = keys1[i];
      key k2 = keys2[j];
      if(get<1>(k1) == get<0>(k2)) {
        vector<key> d_keys = keys(d_);
        key k = make_pair(get<0>(k1), get<1>(k2));
        
        if(find(d_keys.begin(), d_keys.end(), k) != d_keys.end()) d_[k] += d[k1]*m.d[k2];
        else d_[k] = d[k1]*m.d[k2];
      }
    }
  }
  
  return SparseTensor(r,m.c,d_);
}

SparseTensor SparseTensor::kronWith(SparseTensor m) {
  
  dok d_;
  
  
  vector<key> keys1 = keys(d);
  vector<key> keys2 = keys(m.d);
  
  cxd* vals = new cxd[keys1.size()*keys2.size()];
  key* keys = new key[keys1.size()*keys2.size()];
  
  
  #pragma omp parallel for
  for(int i = 0; i < keys1.size(); i++) {
    #pragma omp parallel for
    for(int j = 0; j < keys2.size(); j++) {
      key k1 = keys1[i];
      key k2 = keys2[j];
//      key k = make_pair(get<0>(k1)*m.r+get<0>(k2), get<1>(k1)*m.c+get<1>(k2));
//      d_[k] = d[k1] * m.d[k2];
      keys[i*keys2.size()+j] = make_pair(get<0>(k1)*m.r+get<0>(k2), get<1>(k1)*m.c+get<1>(k2));
      vals[i*keys2.size()+j] = d[k1] * m.d[k2];
    }
  }
  
  for(int i = 0; i < keys1.size()*keys2.size(); i++)
    d_[keys[i]] = vals[i];
  
  return SparseTensor(r*m.r,c*m.c,d_);
}

Tensor SparseTensor::dense() {
  Tensor t (r,c);
  
  for(auto const& k : keys(d)) {
    t.setElementAt(get<0>(k), get<1>(k), d[k]);
  }
  
  return t;
}

vector<key> SparseTensor::keys(dok const& m) {
  vector<key> result;
  for (auto const& i : m) {
    result.push_back(i.first);
  }
  return result;
}
