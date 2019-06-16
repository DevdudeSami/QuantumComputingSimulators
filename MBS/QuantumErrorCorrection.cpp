//
//  QuantumErrorCorrection.cpp
//  MatrixBasedSimulator
//
//  Created by Youssef Moawad on 30/12/2018.
//  Copyright © 2018 Youssef Moawad. All rights reserved.
//

#include "QuantumErrorCorrection.hpp"

vector<ApplicableGate> BitFlipEncodeGates(vector<QID> qIDs) {
  assert(qIDs.size() == 3);
  
  return {
    CNOTAGate({qIDs[0],qIDs[1]}),
    CNOTAGate({qIDs[0],qIDs[2]})
  };
}

vector<ApplicableGate> BitFlipDecodeGates(vector<QID> qIDs) {
  assert(qIDs.size() == 3);
  
  return {
    CNOTAGate({qIDs[0],qIDs[1]}),
    CNOTAGate({qIDs[0],qIDs[2]}),
    TOFFAGate({qIDs[2],qIDs[1],qIDs[0]})
  };
}


void BitFlipCodeTest(QComputer *comp, float flipProbability) {
  assert(comp->numberOfQubits() == 3);
  
  vector<ApplicableGate> gates;
  
  // Encode
  vector<ApplicableGate> encodeGates = BitFlipEncodeGates(comp->allQubits());
  gates.insert(gates.end(), encodeGates.begin(), encodeGates.end());
  
  // Randomly apply an X based on the given flipProbability for each qubit
  for(uint i = 0; i < 3; i++) {
    random_device rd;
    default_random_engine generator(rd());
    uniform_real_distribution<float> distribution(0.0,1.0);
    float randFloat = distribution(generator);
    
    if(randFloat <= flipProbability) gates.push_back(XAGate({i}));
  }
  
  // Decode
  vector<ApplicableGate> decodeGates = BitFlipDecodeGates(comp->allQubits());
  gates.insert(gates.end(), decodeGates.begin(), decodeGates.end());
  
  CircuitOptimiser co (comp, gates);
  co.executeCircuit();
}

vector<ApplicableGate> PhaseFlipEncodeGates(vector<QID> qIDs) {
  assert(qIDs.size() == 3);
  
  return {
    CNOTAGate({qIDs[0],qIDs[1]}),
    CNOTAGate({qIDs[0],qIDs[2]}),
    HAGate({qIDs[0]}),
    HAGate({qIDs[1]}),
    HAGate({qIDs[2]})
  };
}

vector<ApplicableGate> PhaseFlipDecodeGates(vector<QID> qIDs) {
  assert(qIDs.size() == 3);
  
  return {
    HAGate({qIDs[0]}),
    HAGate({qIDs[1]}),
    HAGate({qIDs[2]}),
    CNOTAGate({qIDs[0],qIDs[1]}),
    CNOTAGate({qIDs[0],qIDs[2]}),
    TOFFAGate({qIDs[2],qIDs[1],qIDs[0]})
  };
}

void PhaseFlipCodeTest(QComputer *comp, float flipProbability) {
  assert(comp->numberOfQubits() == 3);
  
  vector<ApplicableGate> gates;
  
  // Encode
  vector<ApplicableGate> encodeGates = PhaseFlipEncodeGates(comp->allQubits());
  gates.insert(gates.end(), encodeGates.begin(), encodeGates.end());
  
  // Randomly apply Z based on the given flipProbability for each qubit
  for(uint i = 0; i < 3; i++) {
    random_device rd;
    default_random_engine generator(rd());
    uniform_real_distribution<float> distribution(0.0,1.0);
    float randFloat = distribution(generator);
    
    if(randFloat <= flipProbability) gates.push_back(ZAGate({i}));
  }
  
  // Decode
  vector<ApplicableGate> decodeGates = PhaseFlipDecodeGates(comp->allQubits());
  gates.insert(gates.end(), decodeGates.begin(), decodeGates.end());
  
  CircuitOptimiser co (comp, gates);
  co.executeCircuit();
}

vector<ApplicableGate> ShorEncodeGates(vector<QID> qIDs) {
  assert(qIDs.size() == 9);
  
  vector<ApplicableGate> gates;
  
  vector<ApplicableGate> nextBatch = PhaseFlipEncodeGates({qIDs[0],qIDs[3],qIDs[6]});
  gates.insert(gates.end(), nextBatch.begin(), nextBatch.end());
  
  nextBatch = BitFlipEncodeGates({qIDs[0],qIDs[1],qIDs[2]});
  gates.insert(gates.end(), nextBatch.begin(), nextBatch.end());
  
  nextBatch = BitFlipEncodeGates({qIDs[3],qIDs[4],qIDs[5]});
  gates.insert(gates.end(), nextBatch.begin(), nextBatch.end());
  
  nextBatch = BitFlipEncodeGates({qIDs[6],qIDs[7],qIDs[8]});
  gates.insert(gates.end(), nextBatch.begin(), nextBatch.end());
  
  return gates;
}

vector<ApplicableGate> ShorDecodeGates(vector<QID> qIDs) {
  assert(qIDs.size() == 9);
  
  vector<ApplicableGate> gates;
  
  vector<ApplicableGate> nextBatch = BitFlipDecodeGates({qIDs[0],qIDs[1],qIDs[2]});
  gates.insert(gates.end(), nextBatch.begin(), nextBatch.end());
  
  nextBatch = BitFlipDecodeGates({qIDs[3],qIDs[4],qIDs[5]});
  gates.insert(gates.end(), nextBatch.begin(), nextBatch.end());
  
  nextBatch = BitFlipDecodeGates({qIDs[6],qIDs[7],qIDs[8]});
  gates.insert(gates.end(), nextBatch.begin(), nextBatch.end());
  
  nextBatch = PhaseFlipDecodeGates({qIDs[0],qIDs[3],qIDs[6]});
  gates.insert(gates.end(), nextBatch.begin(), nextBatch.end());
  
  
  return gates;
}

void ShorCodeTest(QComputer *comp, float flipProbability) {
  assert(comp->numberOfQubits() == 9);
  
  vector<ApplicableGate> gates;
  
  // Encode
  vector<ApplicableGate> encodeGates = ShorEncodeGates(comp->allQubits());
  gates.insert(gates.end(), encodeGates.begin(), encodeGates.end());
  
  // Randomly apply X and/or Z based on the given flipProbability for each qubit
  for(uint i = 0; i < 9; i++) {
    random_device rd;
    default_random_engine generator(rd());
    uniform_real_distribution<float> distribution(0.0,1.0);
    float bitRandFloat = distribution(generator);
    float phaseRandFloat = distribution(generator);
    
    if(bitRandFloat <= flipProbability) gates.push_back(XAGate({i}));
    if(phaseRandFloat <= flipProbability) gates.push_back(ZAGate({i}));
  }
  
  // Decode
  vector<ApplicableGate> decodeGates = ShorDecodeGates(comp->allQubits());
  gates.insert(gates.end(), decodeGates.begin(), decodeGates.end());
  
  CircuitOptimiser co (comp, gates);
  co.executeCircuit();
  
}
