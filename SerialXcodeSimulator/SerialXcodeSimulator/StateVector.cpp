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

StateVector::StateVector(SparseTensor amps, vector<int> ids) : n(ids.size()), amplitudes(amps), qIDs(ids) {
  
  assert(amplitudes.rowCount() == 1);
  assert(log2(amplitudes.colCount()) == n);
  assert(amplitudes.isNormalised());
  
}

vector<double> StateVector::probabilities() {
  vector<double> result;
  amplitudes.enumerateElements([&result](int r, int c, cxd amp) { result.push_back(norm(amp)); } );
  return result;
}

void StateVector::applyGate(SparseTensor t) {
  amplitudes = t.multiplyTo(amplitudes.transpose()).transpose();
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
  SparseTensor newAmplitudes = amplitudes.kronWith(v.amplitudes);
  vector<int> newQIDs = {};
  newQIDs.insert(newQIDs.end(), qIDs.begin(), qIDs.end());
  newQIDs.insert(newQIDs.end(), v.qIDs.begin(), v.qIDs.end());
  return StateVector(newAmplitudes, newQIDs);
}

// indices is the indices in registerIndices (in the state vector), not the ids of the qubits
SparseTensor StateVector::prepareOperator(SparseTensor t, vector<int> indices) {
  for(int i = 1; i < indices.size(); i++) assert(indices[i] == indices[i-1] + 1);
  
  SparseTensor op (0,0,0,{},{});
  
  if(indices[0] == 0) {
    op = t;
    for(int i = indices.size(); i < n; i++) op = op.kronWith(I());
    return op;
  }
  
  op = I();
  for(int i = 1; i < indices.size(); i++) {
    if(i == indices[0]) op = op.kronWith(t);
    else if(find(indices.begin(), indices.end(), i) != indices.end()) {}
    else op = op.kronWith(I());
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
    SparseTensor op = prepareOperator(SWAP(), {i, i+1});
    applyGate(op);
    i++;
  }
  
  i = q2PositionInVector - 1;
  while(i > q2PositionInVector - steps) {
    SparseTensor op = prepareOperator(SWAP(), {i-1, i});
    applyGate(op);
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
