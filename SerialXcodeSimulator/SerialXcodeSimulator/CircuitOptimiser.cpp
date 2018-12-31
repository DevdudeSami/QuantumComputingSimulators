//
//  CircuitOptimiser.cpp
//  SerialXcodeSimulator
//
//  Created by Youssef Moawad on 25/12/2018.
//  Copyright © 2018 Youssef Moawad. All rights reserved.
//

#include "CircuitOptimiser.hpp"


CircuitOptimiser::CircuitOptimiser(QComputer* comp, vector<ApplicableGate> gates) : comp{comp}, gates{gates} {
  for(QID i = 0; i < comp->numberOfQubits(); i++) {
    stateVectorsQubitIDs.push_back({i});
  }
}

CircuitOptimiser::CircuitOptimiser(uint n, vector<ApplicableGate> gates) : comp{new QComputer(n)}, gates{gates} {
  for(QID i = 0; i < n; i++) {
    stateVectorsQubitIDs.push_back({i});
  }
}

void CircuitOptimiser::addGates(vector<ApplicableGate> gates) {
  this->gates.insert(this->gates.end(), gates.begin(), gates.end());
}

list_index CircuitOptimiser::combineTwoQubits(QID q1, QID q2) {
  list_index q1Index = listIndexFromQID(q1);
  list_index q2Index = listIndexFromQID(q2);
  
  // if already combined, do nothing
  if(q1Index == q2Index) return q1Index;
  
  vector<unsigned int> newQIDs = {};
  newQIDs.insert(newQIDs.end(), stateVectorsQubitIDs[q1Index].begin(), stateVectorsQubitIDs[q1Index].end());
  newQIDs.insert(newQIDs.end(), stateVectorsQubitIDs[q2Index].begin(), stateVectorsQubitIDs[q2Index].end());
  
  list_index indexOfCombinedState = min(q1Index, q2Index);
  list_index indexOfDeletedState = max(q1Index, q2Index);
  stateVectorsQubitIDs[indexOfCombinedState] = newQIDs;
  stateVectorsQubitIDs.erase(stateVectorsQubitIDs.begin() + indexOfDeletedState);
  
  return indexOfCombinedState;
}

list_index CircuitOptimiser::combineQubits(vector<QID> qIDs) {
  list_index finalCombinedStateIndex = listIndexFromQID(qIDs[0]);
  QID firstQID = qIDs[0];
  for(QID qID: qIDs) {
    if(qID == firstQID) continue;
    finalCombinedStateIndex = combineTwoQubits(firstQID, qID);
  }
  
  return finalCombinedStateIndex;
}

list_index listIndexFromQIDInState(vector<vector<QID>> state, QID qID) {
  for(list_index i = 0; i < state.size(); i++) {
    vector<QID> qubitIDs = state[i];
    if (find(qubitIDs.begin(), qubitIDs.end(), qID) != qubitIDs.end()) {
      return i;
    }
  }
  
  throw exception();
}

list_index CircuitOptimiser::listIndexFromQID(QID qID) {
  for(list_index i = 0; i < stateVectorsQubitIDs.size(); i++) {
    vector<QID> qubitIDs = stateVectorsQubitIDs[i];
    if (find(qubitIDs.begin(), qubitIDs.end(), qID) != qubitIDs.end()) {
      return i;
    }
  }
  
  throw exception();
}

Tensor* prepareOperatorToRunAtStartOfStateVector(Tensor* t, uint n) {
  uint numberOfAppliedQubits = log2(t->colCount());
  
  // there will be a single identity (dimension 2^(n - numberOfAppliedQubits)) to kron with t
  SparseTensor I = IGate(pow(2, n-numberOfAppliedQubits));
  
  return new SparseTensor(t->sparseKronWith(I));
}

Tensor* prepareOperatorToRunInMiddleOfStateVector(Tensor* t, uint startingIndex, uint n) {
  uint numberOfAppliedQubits = log2(t->colCount());
  
  // there will be two identities (dimensions 2^startingIndex and 2^(n-startingIndex-numberOfAppliedQubits) to kron with t before and after
  SparseTensor I1 = IGate(pow(2, startingIndex));
  SparseTensor I2 = IGate(pow(2, n-numberOfAppliedQubits-startingIndex));
  
  return new SparseTensor(I1.sparseKronWith(t).sparseKronWith(I2));
}

vector<Step> CircuitOptimiser::swapSteps(list_index stateVectorIndex, uint q1ID, uint q2ID) {
  if(q1ID == q2ID) return {};

  vector<QID> stateVector = stateVectorsQubitIDs[stateVectorIndex];
  
  int q1PositionInVector = find(stateVector.begin(), stateVector.end(), q1ID) - stateVector.begin();
  int q2PositionInVector = find(stateVector.begin(), stateVector.end(), q2ID) - stateVector.begin();

  uint stepsCount = abs(q1PositionInVector-q2PositionInVector);
  
  if(q1PositionInVector > q2PositionInVector) {
    int temp = q1ID;
    q1ID = q2ID;
    q2ID = temp;
    temp = q1PositionInVector;
    q1PositionInVector = q2PositionInVector;
    q2PositionInVector = temp;
  }

  SparseTensor* SWAP = new SparseTensor(SWAPGate());
  Step steps[stepsCount*2-1];
  
  // TODO: Potentially combine these two loops
  for(uint i = q1PositionInVector; i < q1PositionInVector + stepsCount; i++) {
    ApplicableGate g = ApplicableGate(SWAP, {stateVector[i],stateVector[i+1]});
    steps[i-q1PositionInVector] = Step(stateVectorsQubitIDs, g);
    swap(stateVector[i],stateVector[i+1]);
    stateVectorsQubitIDs[stateVectorIndex] = stateVector;
  }
  
  for(uint i = q2PositionInVector - 1; i > q2PositionInVector - stepsCount; i--) {
    ApplicableGate g = ApplicableGate(SWAP, {stateVector[i-1],stateVector[i]});
    int indexInArray = -i + q2PositionInVector - 1 + stepsCount;
    steps[indexInArray] = Step(stateVectorsQubitIDs, g);
    swap(stateVector[i-1],stateVector[i]);
    stateVectorsQubitIDs[stateVectorIndex] = stateVector;
  }
  
  return vector<Step>(steps, steps + sizeof steps / sizeof steps[0]);
}

void CircuitOptimiser::executeCircuit() {
  unordered_map<int, vector<QID>> qubitsToCombine;
  vector<Step> steps;
  
  // CALCULATE TOTAL NUMBER OF GATES TO APPLY (INCLUDING SWAPS)
  for(int i = 0; i < gates.size(); i++) {
    if(gates[i].second.size() == 1) {
      steps.push_back(Step(stateVectorsQubitIDs, gates[i]));
      continue;
    }
    
    list_index index = combineQubits(gates[i].second);
    qubitsToCombine[steps.size()] = gates[i].second;
    
    // Swaps
    vector<pair<int, int>> swapsDone;
    for(int j = 0; j < gates[i].second.size(); j++) {
      swapsDone.push_back(make_pair(gates[i].second[j], stateVectorsQubitIDs[index][j]));
      vector<Step> swaps = swapSteps(index, gates[i].second[j], stateVectorsQubitIDs[index][j]);
      steps.insert(steps.end(), swaps.begin(), swaps.end());
    }
    
    steps.push_back(Step(stateVectorsQubitIDs, gates[i]));
    
    // Swap back in reverse order
//    for(int j = gates[i].second.size() - 1; j >= 0; j--) {
//      vector<Step> swaps = swapSteps(index, swapsDone[j].first, swapsDone[j].second);
//      steps.insert(steps.end(), swaps.begin(), swaps.end());
//    }
  }
  
  StateVectorApplicableGate ops[steps.size()];
  
  #pragma omp parallel for
  for(int i = 0; i < steps.size(); i++) {
    ApplicableGate gate = steps[i].second;
    vector<vector<QID>> state = steps[i].first;
    
    list_index li = listIndexFromQIDInState(state, gate.second[0]);
    
    if(gate.second.size() == 1 && state[li].size() == 1) {
      ops[i] = StateVectorApplicableGate(gate.first, li);
      continue;
    }
    
    uint startingIndex = find(state[li].begin(), state[li].end(), gate.second[0]) - state[li].begin();
    ops[i] = StateVectorApplicableGate(prepareOperatorToRunInMiddleOfStateVector(gate.first, startingIndex, state[li].size()), li);
  }
  
  for(int i = 0; i < steps.size(); i++) {
    if(qubitsToCombine.find(i) != qubitsToCombine.end())
      comp->combineQubits(qubitsToCombine[i]);
    comp->applyPreparedGate(ops[i].second, ops[i].first);
    comp->setStateVectorQIDs(ops[i].second, steps[i].first[ops[i].second]);
  }
}
