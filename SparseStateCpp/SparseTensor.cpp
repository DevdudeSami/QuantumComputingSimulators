//
//  SparseTensor.cpp
//  SerialXcodeSimulator
//
//  Created by Youssef Moawad on 29/11/2018.
//  Copyright © 2018 Youssef Moawad. All rights reserved.
//

#include "SparseTensor.hpp"
#include <omp.h>

SparseTensor::SparseTensor(unsigned int r, unsigned int c, unsigned int nnz, key* keys, cxd* vals) : r{r},  c{c}, nnz{nnz} {
  this->keys = new key[nnz];
  this->vals = new cxd[nnz];
  
  copy(keys, keys+nnz, this->keys);
  copy(vals, vals+nnz, this->vals);
}

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

SparseTensor SparseTensor::addTo(SparseTensor t) {
  assert(r == t.r);
  assert(c == t.c);
  
  vector<cxd> values;
  vector<key> ks;
  unsigned int new_nnz = 0;

  #pragma omp parallel for
  for(int i = 0; i < nnz; i++) {
    if(find(t.keys, t.keys + t.nnz, keys[i]) != t.keys + t.nnz) {
      ks.push_back(keys[i]);
      values.push_back(vals[i]+t.vals[i]);
      new_nnz++;
    }
    else {
      ks.push_back(keys[i]);
      values.push_back(vals[i]);
      new_nnz++;
    }
  }
  
  #pragma omp parallel for
  for(int i = 0; i < t.nnz; i++) {
    if(find(keys, keys+nnz, t.keys[i]) == keys+nnz) {
      ks.push_back(t.keys[i]);
      values.push_back(t.vals[i]);
      new_nnz++;
    }
  }
  
  cxd *new_vals = new cxd[new_nnz];
  key *new_keys = new key[new_nnz];
  
  copy(values.begin(), values.end(), new_vals);
  copy(ks.begin(), ks.end(), new_keys);
  
  return SparseTensor(r,c,nnz,new_keys,new_vals);
}

unsigned int SparseTensor::rowCount() {
  return r;
}

unsigned int SparseTensor::colCount() {
  return c;
}

SparseTensor SparseTensor::multiplyTo(cxd s) {

  key* new_keys = new key[nnz];
  cxd* new_vals = new cxd[nnz];
  
  #pragma omp parallel for
  for(int i = 0; i < nnz; i++) {
    new_keys[i] = keys[i];
    new_vals[i] = s*vals[i];
  }

  return SparseTensor(r,c,nnz,new_keys,new_vals);
}

SparseTensor SparseTensor::multiplyTo(SparseTensor t) {
  assert(c == t.r);

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
    for(int j = 0; j < t.nnz; j++) {
      key k1 = keys[i];
      key k2 = t.keys[j];
      if(k1.second == k2.first) {
        key k = make_pair(k1.first, k2.second);

        ptrdiff_t keyIndex = distance(ks.begin(), find(ks.begin(), ks.end(), k));
        
        if(keyIndex < ks.size()) {
          values[keyIndex] += vals[i]*t.vals[j];
        }
        else {
          new_nnz++;
          ks.push_back(k);
          values.push_back(vals[i]*t.vals[j]);
        }
      }
    }
  }
  
  cxd *new_vals = new cxd[new_nnz];
  key *new_keys = new key[new_nnz];
  
  copy(values.begin(), values.end(), new_vals);
  copy(ks.begin(), ks.end(), new_keys);

  return SparseTensor(r,t.c,new_nnz,new_keys,new_vals);
}

SparseTensor SparseTensor::kronWith(SparseTensor t) {

  key* new_keys = new key[nnz*t.nnz];
  cxd* new_vals = new cxd[nnz*t.nnz];

  #pragma omp parallel for
  for(int i = 0; i < nnz; i++) {
    #pragma omp parallel for
    for(int j = 0; j < t.nnz; j++) {
      key k1 = keys[i];
      key k2 = t.keys[j];

      new_keys[i*t.nnz+j] = make_pair(k1.first*t.r+k2.first, k1.second*t.c+k2.second);
      new_vals[i*t.nnz+j] = vals[i] * t.vals[j];
    }
  }

  return SparseTensor(r*t.r,c*t.c,nnz*t.nnz,new_keys,new_vals);
}

cxd SparseTensor::dotProductWith(SparseTensor t) {
  assert(matchesDimensionsWith(t));
  
  cxd result = 0;
  
  #pragma omp parallel for reduction (+:result)
  for(int i = 0; i < nnz; i++) {
    cxd e = t.elementAt(keys[i].first, keys[i].second);
    if(e != cxd(0)) result += e*vals[i];
  }
  
  return result;
}

Tensor SparseTensor::dense() {
  Tensor t (r,c);
  
  #pragma omp parallel for
  for(int i = 0; i < nnz; i++) {
    key k = keys[i];
    t.setElementAt(k.first, k.second, vals[i]);
  }
  
  return t;
}

cxd SparseTensor::elementAt(unsigned int i, unsigned int j) {
  key k = make_pair(i, j);

  for(int i = 0; i < nnz; i++) {
    if(keys[i] == k) return vals[i];
  }
  
  return 0;
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

bool SparseTensor::matchesDimensionsWith(SparseTensor t) {
  return r == t.r && c == t.c;
}

bool SparseTensor::isNormalised() {
  double sumOfNorms = 0;
  
  #pragma omp parallel for reduction (+:sumOfNorms)
  for(int i = 0; i < nnz; i++)
    sumOfNorms += pow(abs(vals[i]),2);
  
  return round(sumOfNorms*100000)/100000 == 1;
}

bool SparseTensor::elementIsNonZero(unsigned int i, unsigned int j) {
  key k = make_pair(i, j);
  
  return find(keys, keys+nnz, k) != keys+nnz;
}

void SparseTensor::enumerateElements(function<void(int,int,cxd)> f) {
  for(int i = 0; i < r; i++)
    for(int j = 0; j < c; j++)
      f(i,j,elementAt(i, j));
}

void SparseTensor::enumerateNNZElements(function<void(int,int,cxd)> f) {
  for(int i = 0; i < nnz; i++)
    f(keys[i].first,keys[i].second,vals[i]);
}

string SparseTensor::toString() {
  string result = "";
  
  for(int i = 0; i < r; i++) {
    for(int j = 0; j < c; j++) {
      cxd val = elementAt(i,j);
      result += to_string(val.real()) + (val.imag() != 0 ? " + " + to_string(val.imag()) + "i" : "") + "\t";
    }
    result += "\n";
  }
  
  return result;
}

//vector<key> SparseTensor::keys(dok const& m) {
//  vector<key> result;
//  for (auto const& i : m) {
//    result.push_back(i.first);
//  }
//  return result;
//}
