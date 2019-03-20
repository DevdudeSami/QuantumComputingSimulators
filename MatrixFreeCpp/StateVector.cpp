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

int nnthCleared(int n, vector<int> ts) {
  int mask = (1 << 20) - 1;
  
  for(int i = 0; i < ts.size(); i++)
    mask &= ~(1 << ts[i]);
  int notMask = ~mask;
  
  return (n & mask) | ((n & notMask) << ts.size());
}

int nthInSequence(int n, vector<int> ts, int s) {
  if(s >= pow(2,ts.size())) throw "Unapplicable";
  
  int f = nnthCleared(n, ts);
  
  for(int i = 0; i < ts.size(); i++) {
    if(!(s & (1<<i))) {
      f |= 1 << ts[i];
    }
  }
  
  return f;
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

void StateVector::applyMultiGate(vector<QID> qIDs, Gate gate) {
  vector<int> qIndices;
  qIndices.reserve(qIDs.size());

  for(int i = 0; i < qIDs.size(); i++) {
    qIndices.push_back(distance(this->qIDs, find(this->qIDs, this->qIDs+n, qIDs[i])));
  }
  
  int N = pow(2,qIDs.size()); // size of gate
  
//  #pragma omp parallel for
  for(int i = 0; i < pow(2,n-qIDs.size()); i++) {
    vector<int> states;
    vector<cxd> amps;

    for(int j = 0; j < N; j++) {
      int state = nthInSequence(i, qIndices, j);
      states.push_back(state);
      amps.push_back(amplitudes[state]);
    }
    
//    #pragma omp parallel for
    for(int i = 0; i < N; i++) {
      cxd sum = 0;
      for(int j = 0; j < N; j++) {
        sum += gate[N*i+j]*amps[j];
      }
      amplitudes[states[i]] = sum;
    }
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

string StateVector::measure() {
  vector<string> combs = qubitStatesCombinations(n);
  double* probs = probabilities();

  random_device rd;
  mt19937 gen(rd());
  discrete_distribution<> d (probs, probs+(int)pow(2,n));

  string m = combs[d(gen)];

  reverse(m.begin(), m.end());

  return m;
}

StateVector StateVector::combineWith(StateVector v) {
  cxd* newAmplitudes = new cxd[pow(2, n+v.n)];

  uint N1 = pow(2, n);
  uint N2 = pow(2, v.n);
  
  // do a kronecker product on the amplitude vectors
  #pragma omp parallel for
  for(int i = 0; i < N2; i++) {
    #pragma omp parallel for
    for(int j = 0; j < N1; j++) {
      newAmplitudes[i*(N1)+j] = v.amplitudes[i] * amplitudes[j];
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


// - MARK: Unused functions
void StateVector::swapAdjacentQubits(list_index q1Index) {
  if(q1Index == n-1) {
    throw "Can't perform this swap";
  }
  
  applyMultiGate({qIDs[q1Index], qIDs[q1Index+1]}, SWAP());
  swap(qIDs[q1Index], qIDs[q1Index+1]);
}

void StateVector::move(list_index srcIndex, list_index desIndex) {
  if(srcIndex == desIndex) return;
  
  uint stepsCount = sqrt(pow(int(srcIndex)-int(desIndex),2));
  
  if(srcIndex > desIndex) {
    for(uint i = srcIndex; i > srcIndex - stepsCount; i--) {
      swapAdjacentQubits(i-1);
    }
  } else {
    for(uint i = srcIndex; i < srcIndex + stepsCount; i++) {
      swapAdjacentQubits(i);
    }
  }
}

list_index StateVector::makeAdjacent(vector<QID> qIDs) {
  list_index qIndices[qIDs.size()];
  
  for(int i = 0; i < qIDs.size(); i++) {
    qIndices[i] = distance(this->qIDs, find(this->qIDs, this->qIDs+n, qIDs[i]));
  }
  
  // check if already adjacent
  bool adjacent = true;
  for(int i = 1; i < qIDs.size(); i++) {
    if(qIndices[i] - qIndices[i-1] != 1) {
      adjacent = false;
      break;
    }
  }
  if(adjacent) return qIndices[0];
  
  list_index q1Index = qIndices[0];
  
  // check if there's enough room after the first qubit for the rest of the qubits
  if(q1Index + qIDs.size() > n) {
    // not enough room
    // move the first qubit back enough steps
    uint stepsCount = q1Index + qIDs.size() - n;
    move(q1Index, q1Index-stepsCount);
    q1Index -= stepsCount;
  }
  // now there's enough room, move everything in front of the first qubit, starting from the end
  for(int i = qIDs.size() - 1; i >= 1; i--) {
    list_index index = distance(this->qIDs, find(this->qIDs, this->qIDs+n, qIDs[i]));
    move(index, q1Index+i);
  }
  // the first qubit may have gotten moved; check and move it back
  list_index q1NewIndex = distance(this->qIDs, find(this->qIDs, this->qIDs+n, qIDs[0]));
  if(q1Index != q1NewIndex) move(q1NewIndex, q1Index);
  
  return q1Index;
}
