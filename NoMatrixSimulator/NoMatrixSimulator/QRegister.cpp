//
//  QRegister.cpp
//  NoMatrixSimulator
//
//  Created by Youssef Moawad on 05/02/2019.
//  Copyright © 2019 Youssef Moawad. All rights reserved.
//

#include "QRegister.hpp"

int nthCleared(int n, int t) {
  int mask = (1 << t) - 1;
  int notMask = ~mask;
  
  return (n & mask) | ((n & notMask) << 1);
}

QRegister::QRegister(uint n) : n{n} {
  amplitudes = new cxd[pow(2,n)];
  amplitudes[0] = cxd(1);
}

void QRegister::applySingleGate(QID qID, cxd gate[4]) {
  #pragma omp parallel for
  for(int i = 0; i < pow(2,n-1); i++) {
    int zero_state = nthCleared(i, qID);
    int one_state = zero_state | (1 << qID);
    
    cxd zero_amp = amplitudes[zero_state];
    cxd one_amp = amplitudes[one_state];
    
    amplitudes[zero_state] = gate[0]*zero_amp + gate[1]*one_amp;
    amplitudes[one_state] = gate[2]*zero_amp + gate[3]*one_amp;
  }
}

void QRegister::applyCnGate(vector<QID> controls, QID qID, cxd gate[4]) {
  #pragma omp parallel for
  for(int i = 0; i < pow(2,n-1); i++) {
    int zero_state = nthCleared(i, qID);
    int one_state = zero_state | (1 << qID);
    
    cxd zero_amp = amplitudes[zero_state];
    cxd one_amp = amplitudes[one_state];
    
    bool control_zero = true;
    bool control_one = true;
    
    for(int c = 0; c < controls.size(); c++) {
      if(! (((1 << controls[c]) & zero_state) > 0)) control_zero = false;
      if(! (((1 << controls[c]) & one_state) > 0)) control_one = false;
      
      if(!control_zero && !control_one) break;
    }
    
    if(control_zero) amplitudes[zero_state] = gate[0]*zero_amp + gate[1]*one_amp;
    if(control_one) amplitudes[one_state] = gate[2]*zero_amp + gate[3]*one_amp;
  }
}

vector<string> product(vector<string> v1, vector<string> v2) {
  vector<string> results = {};
  
  for(string i: v1)
    for(string j: v2)
      results.push_back(i + j);
  
  return results;
}

vector<string> qubitStatesCombinations(unsigned int n) {
  vector<string> results = {"0", "1"};
  
  for(int i = 0; i < n-1; i++)
    results = product(results, vector<string>({"0","1"}));
  
  return results;
}

string QRegister::measure() {
  
  vector<string> combs = qubitStatesCombinations(n);
  vector<double> probs;
  
  for(int i = 0; i < pow(2,n); i++) probs.push_back(norm(amplitudes[i]));
  
  random_device rd;
  mt19937 gen(rd());
  discrete_distribution<> d (probs.begin(), probs.end());
  
  return combs[d(gen)];
}
