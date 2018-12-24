//
//  StateVector.cpp
//  SerialXcodeSimulator
//
//  Created by Youssef Moawad on 09/11/2018.
//  Copyright © 2018 Youssef Moawad. All rights reserved.
//

#include "StateVector.hpp"
#include "Gates.hpp"
#include <random>

using namespace std;

vector<string> qubitStatesCombinations(unsigned int n);

StateVector::StateVector(SparseTensor amps, vector<unsigned int> ids) : n(ids.size()), amplitudes(amps), qIDs(ids) {
  
  assert(amplitudes.colCount() == 1);
  assert(log2(amplitudes.rowCount()) == n);
  assert(amplitudes.isNormalised());
  
}

vector<double> StateVector::probabilities() {
  vector<double> result;
  amplitudes.enumerateElements([&result](int r, int c, cxd amp) { result.push_back(norm(amp)); } );
  return result;
}

unsigned long StateVector::numberOfQubits() { return n; }

vector<unsigned int> StateVector::qubitIDs() { return qIDs; }

void StateVector::applyGate(Tensor* t) {
  amplitudes = t->multiplyTo(amplitudes);
}

void StateVector::applyNGate(Tensor *t, vector<unsigned int> qIDs) {
  vector<pair<int, int>> swapsDone;
  vector<unsigned int> qIndicesToSwapInto;
  
  // Swap into the first qIDs.size() qubits
  for(int i = 0; i < qIDs.size(); i++) {
    qIndicesToSwapInto.push_back(i);
    swapsDone.push_back(make_pair(qIDs[i], this->qIDs[i]));
    swap(qIDs[i], this->qIDs[i]);
  }
  
  Tensor *op = prepareOperator(t, qIndicesToSwapInto);
  applyGate(op);
  
  // Swap back in reverse order
  for(int i = qIDs.size() - 1; i >= 0; i--) {
    swap(swapsDone[i].first, swapsDone[i].second);
  }
}

string StateVector::measure() {
  vector<string> combs = qubitStatesCombinations(n);
  vector<double> probs = probabilities();
  
  random_device rd;
  mt19937 gen(rd());
  discrete_distribution<> d (probs.begin(), probs.end());
  
  return combs[d(gen)];
}

StateVector StateVector::combineWith(StateVector v) {
  SparseTensor newAmplitudes = amplitudes.sparseKronWith(v.amplitudes);
  vector<unsigned int> newQIDs = {};
  newQIDs.insert(newQIDs.end(), qIDs.begin(), qIDs.end());
  newQIDs.insert(newQIDs.end(), v.qIDs.begin(), v.qIDs.end());
  return StateVector(newAmplitudes, newQIDs);
}

// TODO: Check this function for memory leaks; particularly with using new
// indices is the indices in registerIndices (in the state vector), not the ids of the qubits
Tensor *StateVector::prepareOperator(Tensor *t, vector<unsigned int> indices) {
  for(int i = 1; i < indices.size(); i++) assert(indices[i] == indices[i-1] + 1);
  
  Tensor *op;
  
  if(indices[0] == 0) {
    op = t;
    for(int i = indices.size(); i < n; i++) {
      op = new SparseTensor(op->sparseKronWith(IGate()));
    }
    return op;
  }
  
  SparseTensor I = IGate();
  op = &I;
  
  for(int i = 1; i < n; i++) {
    if(i == indices[0]) {
      op = new SparseTensor(op->sparseKronWith(t));
    }
    else if(find(indices.begin(), indices.end(), i) != indices.end()) {}
    else {
      op = new SparseTensor(op->sparseKronWith(I));
    }
  }
  
  return op;
}

void StateVector::swap(uint q1ID, uint q2ID) {
  if(q1ID == q2ID) return;
  int q1PositionInVector = find(qIDs.begin(), qIDs.end(), q1ID) - qIDs.begin();
  int q2PositionInVector = find(qIDs.begin(), qIDs.end(), q2ID) - qIDs.begin();
  
  SparseTensor SWAP = SWAPGate();
  
  uint steps = abs(q1PositionInVector-q2PositionInVector);
  
  if(q1PositionInVector > q2PositionInVector) {
    int temp = q1ID;
    q1ID = q2ID;
    q2ID = temp;
    temp = q1PositionInVector;
    q1PositionInVector = q2PositionInVector;
    q2PositionInVector = temp;
  }
  
  Tensor* ops[steps*2-1];
  
  // TODO: Potentially combine these two loops
  #pragma omp parallel for
  for(uint i = q1PositionInVector; i < q1PositionInVector + steps; i++) {
    ops[i-q1PositionInVector] = prepareOperator(&SWAP, {i,i+1});
  }
  
  #pragma omp parallel for
  for(uint i = q2PositionInVector - 1; i > q2PositionInVector - steps; i--) {
    int indexInArray = -i + q2PositionInVector - 1 + steps;
    ops[indexInArray] = prepareOperator(&SWAP, {i-1,i});
  }

  
  for(int i = 0; i < steps*2-1; i++) {
    applyGate(ops[i]);
  }
  
//  unsigned int i = q1PositionInVector;
//  while(i < q1PositionInVector + steps) {
//    Tensor *op = prepareOperator(&SWAP, {i, i+1});
//    applyGate(op);
//    i++;
//  }
//
//  i = q2PositionInVector - 1;
//  while(i > q2PositionInVector - steps) {
//    Tensor *op = prepareOperator(&SWAP, {i-1, i});
//    applyGate(op);
//    i--;
//  }
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
