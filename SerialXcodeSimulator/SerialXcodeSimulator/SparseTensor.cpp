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

  #pragma omp parallel for
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
  
  #pragma omp parallel for
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

SparseTensor SparseTensor::kMultiplyTo(cxd s) {

  key* new_keys = new key[nnz];
  cxd* new_vals = new cxd[nnz];
  
  #pragma omp parallel for
  for(int i = 0; i < nnz; i++) {
    new_keys[i] = keys[i];
    new_vals[i] = s*vals[i];
  }

  return SparseTensor(r,c,nnz,new_keys,new_vals);
}

SparseTensor SparseTensor::multiplyTo(SparseTensor m) {
  assert(c == m.r);

  vector<cxd> values;
  vector<key> ks;
  unsigned int new_nnz = 0;

  /*
   Each step where a multiplication takes place, need to do all the multiplications required, i.e. find the final result
   that would go into that position. And then not revisit it again. Perhaps we need to first populate the keys of the final
   result, but that's hard to do in an array since we don't know the final nnz.
   
   What we can do is... Have a vector (well, the pair of vectors above) where at each multiplication, you just add in the key,
   even if it already exists. After all the loops, we do some sort of reduce and add all the ones with the same key. This
   wouldn't allow us to calculate nnz however... Unless...
   */

  
//  #pragma omp parallel for
  for(int i = 0; i < nnz; i++) {
//    #pragma omp parallel for
    for(int j = 0; j < m.nnz; j++) {
      key k1 = keys[i];
      key k2 = m.keys[j];
      if(k1.second == k2.first) {
        key k = make_pair(k1.first, k2.second);

        ptrdiff_t keyIndex = distance(ks.begin(), find(ks.begin(), ks.end(), k));
        
        if(keyIndex < ks.size()) {
          values[keyIndex] += vals[i]*m.vals[j];
        }
        else {
          new_nnz++;
          ks.push_back(k);
          values.push_back(vals[i]*m.vals[j]);
        }
      }
    }
  }
  
  cxd *new_vals = new cxd[new_nnz];
  key *new_keys = new key[new_nnz];
  
  copy(values.begin(), values.end(), new_vals);
  copy(ks.begin(), ks.end(), new_keys);

  return SparseTensor(r,m.c,new_nnz,new_keys,new_vals);
}

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

SparseTensor SparseTensor::transpose() {
  
  key* new_keys = new key[nnz];
  cxd* new_vals = new cxd[nnz];
  
  #pragma omp parallel for
  for(int i = 0; i < nnz; i++) {
    new_keys[i] = make_pair(keys[i].second, keys[i].first);
    new_vals[i] = vals[i];
  }
  
  return SparseTensor(c,r,nnz,new_keys,new_vals);
}


//vector<key> SparseTensor::keys(dok const& m) {
//  vector<key> result;
//  for (auto const& i : m) {
//    result.push_back(i.first);
//  }
//  return result;
//}
