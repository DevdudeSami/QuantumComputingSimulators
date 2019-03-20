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

StateVector::StateVector(Tensor amps, vector<int> ids) : n(ids.size()), amplitudes(amps), qIDs(ids) {
  
  assert(amplitudes.rowCount() == 1);
  assert(log2(amplitudes.colCount()) == n);
  assert(amplitudes.isNormalised());
  
}

StateVector::StateVector(uint n) : n(n), amplitudes(Tensor({cxd(1)})) {
  for(uint i = 0; i < n; i++) {
    qIDs.push_back(i);
  }

  vector<cxd> amps = {0};
  for(uint i = 1; i < pow(2,n); i++) {
    amps.push_back(0);
  }

  this->amplitudes = Tensor(amps);
}

uint StateVector::numberOfQubits() { return n; }

vector<double> StateVector::probabilities() {
  vector<double> result;
  amplitudes.enumerateElements([&result](int r, int c, cxd amp) { result.push_back(norm(amp)); } );
  return result;
}

void StateVector::applyGate(Tensor t) {
  amplitudes = t.multiplyBy(amplitudes.transpose()).transpose();
}

void StateVector::applyNGate(Tensor t, vector<unsigned int> qIDs) {
  vector<pair<int, int>> swapsDone;
  vector<int> qIndicesToSwapInto;
  
  // Swap into the first qIDs.size() qubits
  for(int i = 0; i < qIDs.size(); i++) {
    qIndicesToSwapInto.push_back(i);
    swapsDone.push_back(make_pair(qIDs[i], this->qIDs[i]));
    swap(qIDs[i], this->qIDs[i]);
  }
  
  Tensor op = prepareOperator(t, qIndicesToSwapInto);
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
  Tensor newAmplitudes = amplitudes.kronWith(v.amplitudes);
  vector<int> newQIDs = {};
  newQIDs.insert(newQIDs.end(), qIDs.begin(), qIDs.end());
  newQIDs.insert(newQIDs.end(), v.qIDs.begin(), v.qIDs.end());
  return StateVector(newAmplitudes, newQIDs);
}

// indices is the indices in registerIndices (in the state vector), not the ids of the qubits
Tensor StateVector::prepareOperator(Tensor t, vector<int> indices) {
  for(int i = 1; i < indices.size(); i++) assert(indices[i] == indices[i-1] + 1);
  
  Tensor op (0,0);
  
  if(indices[0] == 0) {
    op = t;
    for(int i = indices.size(); i < n; i++) op = op.kronWith(IGate());
    return op;
  }
  
  op = IGate();
  for(int i = 1; i < indices.size(); i++) {
    if(i == indices[0]) op = op.kronWith(t);
    else if(find(indices.begin(), indices.end(), i) != indices.end()) {}
    else op = op.kronWith(IGate());
  }
  
  return op;
}

void StateVector::swap(uint q1ID, uint q2ID) {
  if(q1ID == q2ID) return;
  
  int q1PositionInVector = find(qIDs.begin(), qIDs.end(), q1ID) - qIDs.begin();
  int q2PositionInVector = find(qIDs.begin(), qIDs.end(), q2ID) - qIDs.begin();
  
  uint steps = abs(q1PositionInVector-q2PositionInVector);
  
  if(q1PositionInVector > q2PositionInVector) {
    int temp = q1ID;
    q1ID = q2ID;
    q2ID = temp;
    temp = q1PositionInVector;
    q1PositionInVector = q2PositionInVector;
    q2PositionInVector = temp;
  }
  
  int i = q1PositionInVector;
  while(i < q1PositionInVector + steps) {
    Tensor op = prepareOperator(SWAPGate(), {i, i+1});
    applyNGate(op, {static_cast<unsigned int>(qIDs[i]), static_cast<unsigned int>(qIDs[i+1])});
    i++;
  }
  
  i = q2PositionInVector - 1;
  while(i > q2PositionInVector - steps) {
    Tensor op = prepareOperator(SWAPGate(), {i-1, i});
    applyNGate(op, {static_cast<unsigned int>(qIDs[i-1]), static_cast<unsigned int>(qIDs[i])});
    i--;
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
