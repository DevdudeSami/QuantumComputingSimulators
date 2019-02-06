//
//  StateVector.cpp
//  NoMatrixSimulator
//
//  Created by Youssef Moawad on 05/02/2019.
//  Copyright © 2019 Youssef Moawad. All rights reserved.
//

#include "StateVector.hpp"

int nthCleared(int n, int t) {
  int mask = (1 << t) - 1;
  int notMask = ~mask;
  
  return (n & mask) | ((n & notMask) << 1);
}

StateVector::StateVector(uint n, uint* qIDs) : n{n} {
  this->qIDs = new uint[n];
  memcpy(this->qIDs, qIDs, sizeof(uint)*n);
  
  amplitudes = new cxd[pow(2,n)];
  amplitudes[0] = cxd(1);
}

void StateVector::setAmplitudes(cxd *newAmplitudes) {
  memcpy(this->amplitudes, newAmplitudes, sizeof(cxd)*pow(2,n));
}

double* StateVector::probabilities() {
  double* probs = new double[(int)pow(2,n)];
  
  for(int i = 0; i < pow(2,n); i++) probs[i] = norm(amplitudes[i]);
  
  return probs;
}

uint StateVector::numberOfQubits() { return n; }
uint* StateVector::qubitIDs() { return qIDs; }

void StateVector::applySingleGate(QID qID, Gate gate) {
  list_index qIndex = distance(qIDs, find(qIDs, qIDs+n, qID));
  
  #pragma omp parallel for
  for(int i = 0; i < pow(2,n-1); i++) {
    int zero_state = nthCleared(i, qIndex);
    int one_state = zero_state | (1 << qIndex);
    
    cxd zero_amp = amplitudes[zero_state];
    cxd one_amp = amplitudes[one_state];
    
    amplitudes[zero_state] = gate[0]*zero_amp + gate[1]*one_amp;
    amplitudes[one_state] = gate[2]*zero_amp + gate[3]*one_amp;
  }
}

void StateVector::applyCnGate(vector<QID> controls, QID qID, Gate gate) {
  list_index qIndex = distance(qIDs, find(qIDs, qIDs+n, qID));
  vector<list_index> controlIndices;
  for(QID c: controls) {
    controlIndices.push_back(distance(qIDs, find(qIDs, qIDs+n, c)));
  }
  
  #pragma omp parallel for
  for(int i = 0; i < pow(2,n-1); i++) {
    int zero_state = nthCleared(i, qIndex);
    int one_state = zero_state | (1 << qIndex);
    
    cxd zero_amp = amplitudes[zero_state];
    cxd one_amp = amplitudes[one_state];
    
    bool control_zero = true;
    bool control_one = true;
    
    for(int c = 0; c < controlIndices.size(); c++) {
      if(! (((1 << controlIndices[c]) & zero_state) > 0)) control_zero = false;
      if(! (((1 << controlIndices[c]) & one_state) > 0)) control_one = false;
      
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

string StateVector::measure() {
  vector<string> combs = qubitStatesCombinations(n);
  double* probs = probabilities();
  
  random_device rd;
  mt19937 gen(rd());
  discrete_distribution<> d (probs, probs+(int)pow(2,n));
  
  return combs[d(gen)];
}

StateVector StateVector::combineWith(StateVector v) {
  cxd* newAmplitudes = new cxd[pow(2, n+v.n)];
  
  // do a kronecker product on the amplitude vectors
  #pragma omp parallel for
  for(int i = 0; i < pow(2,n); i++) {
    #pragma omp parallel for
    for(int j = 0; j < pow(2,v.n); j++) {
      newAmplitudes[i*v.n+j] = amplitudes[i] * v.amplitudes[j];
    }
  }

  QID* newQIDs = new QID[n+v.n];
  
  for(int i = 0; i < n; i++) {
    newQIDs[i] = qIDs[i];
  }
  for(int i = 0; i < v.n; i++) {
    newQIDs[n+i] = v.qIDs[i];
  }
  
  StateVector combined (n+v.n, newQIDs);
  combined.setAmplitudes(newAmplitudes);
  return combined;
}
