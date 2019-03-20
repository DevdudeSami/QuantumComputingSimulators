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

vector<cxd> StateVector::probabilityAmplitudes() {
  vector<cxd> result;
  amplitudes.enumerateElements([&result](int r, int c, cxd amp) { result.push_back(amp); } );
  return result;
}

unsigned long StateVector::numberOfQubits() { return n; }

vector<unsigned int> StateVector::qubitIDs() { return qIDs; }

void StateVector::applyGate(Tensor* t) {
  amplitudes = t->multiplyTo(amplitudes);
}

uint StateVector::makeQubitsAdjacent(vector<uint> targetQIDs) {
  // find the first qubit in the vector
  uint q1Index = find(qIDs.begin(), qIDs.end(), targetQIDs[0]) - qIDs.begin();
  // check if there's enough room for the rest of the qubits after it
  if(q1Index + targetQIDs.size() > qIDs.size()) {
    // not enough room
    // move the first qubit back enough steps
    uint stepsCount = (q1Index + targetQIDs.size()) - qIDs.size();
    moveQubits(qIDs[q1Index], qIDs[q1Index-stepsCount]);
    q1Index -= stepsCount;
  }
  // now there's enough room, move everything in front of the first qubit, starting from the end
  for(int j = targetQIDs.size() - 1; j >= 1; j--) {
    moveQubits(targetQIDs[j], qIDs[q1Index+j]);
  }
  // the first qubit may have gotten moved; check and move it back
  uint q1NewIndex = find(qIDs.begin(), qIDs.end(), targetQIDs[0]) - qIDs.begin();
  if(q1Index != q1NewIndex) {
    moveQubits(qIDs[q1NewIndex], qIDs[q1Index]);
  }

  return q1Index;
}

void StateVector::applyNGate(Tensor *t, vector<unsigned int> qIDs) {
  uint startingIndex = makeQubitsAdjacent(qIDs);
  
  Tensor *op = prepareOperatorToRunInMiddle(t, startingIndex);
  applyGate(op);
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

Tensor* StateVector::prepareOperatorToRunInMiddle(Tensor* t, uint startingIndex) {
  uint numberOfAppliedQubits = log2(t->colCount());
  
  // there will be two identities (dimensions 2^startingIndex and 2^(n-startingIndex-numberOfAppliedQubits) to kron with t before and after
  SparseTensor I1 = IGate(pow(2, startingIndex));
  SparseTensor I2 = IGate(pow(2, n-numberOfAppliedQubits-startingIndex));
  
  return new SparseTensor(I1.sparseKronWith(t).sparseKronWith(I2));
}

Tensor* StateVector::prepareOperatorToRunAtStart(Tensor* t, uint numberOfAppliedQubits) {
  assert(log2(t->colCount()) == numberOfAppliedQubits);
  
  // there will be a single identity (dimension 2^(n - numberOfAppliedQubits)) to kron with t
  SparseTensor I = IGate(pow(2, n-numberOfAppliedQubits));
  
  return new SparseTensor(t->sparseKronWith(I));
}

void StateVector::moveQubits(uint src, uint des) {
  if(src == des) return;

  int q1PositionInVector = find(qIDs.begin(), qIDs.end(), src) - qIDs.begin();
  int q2PositionInVector = find(qIDs.begin(), qIDs.end(), des) - qIDs.begin();
  
  uint stepsCount = abs(q1PositionInVector-q2PositionInVector);
  
  SparseTensor* SWAP = new SparseTensor(SWAPGate());


  if(q1PositionInVector > q2PositionInVector) {
    int indexInArray = 0;
    for(uint i = q1PositionInVector; i > q1PositionInVector - stepsCount; i--) {
      applyNGate(SWAP, {qIDs[i-1], qIDs[i]});
      swap(qIDs[i-1],qIDs[i]);
      indexInArray++;
    }
    
  } else {
    int indexInArray = 0;
    for(uint i = q1PositionInVector; i < q1PositionInVector + stepsCount; i++) {
      applyNGate(SWAP, {qIDs[i], qIDs[i+1]});
      swap(qIDs[i],qIDs[i+1]);
      indexInArray++;
    }
  }
}

// void StateVector::swap(uint q1ID, uint q2ID) {
//   if(q1ID == q2ID) return;
//   int q1PositionInVector = find(qIDs.begin(), qIDs.end(), q1ID) - qIDs.begin();
//   int q2PositionInVector = find(qIDs.begin(), qIDs.end(), q2ID) - qIDs.begin();
  
//   SparseTensor SWAP = SWAPGate();
  
//   uint steps = abs(q1PositionInVector-q2PositionInVector);
  
//   if(q1PositionInVector > q2PositionInVector) {
//     int temp = q1ID;
//     q1ID = q2ID;
//     q2ID = temp;
//     temp = q1PositionInVector;
//     q1PositionInVector = q2PositionInVector;
//     q2PositionInVector = temp;
//   }
  
//  unsigned int i = q1PositionInVector;
//  while(i < q1PositionInVector + steps) {
//    Tensor *op = prepareOperatorToRunInMiddle(&SWAP, i);
//    applyGate(op);
//    i++;
//  }

//  i = q2PositionInVector - 1;
//  while(i > q2PositionInVector - steps) {
//    Tensor *op = prepareOperatorToRunInMiddle(&SWAP, i-1);
//    applyGate(op);
//    i--;
//  }
// }

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

void StateVector::setQIDs(vector<unsigned int> qIDs) {
  this->qIDs = qIDs;
}
