//
//  QRegister.cpp
//  NoMatrixSimulator
//
//  Created by Youssef Moawad on 05/02/2019.
//  Copyright © 2019 Youssef Moawad. All rights reserved.
//

#include "QRegister.hpp"

QRegister::QRegister(uint n) : n{n} {
  for(QID i = 0; i < n; i++) {
    QID qIDs[1] = {i};
    StateVector v (1, qIDs);

    vectors.push_back(v);
  }
}

uint QRegister::numberOfQubits() {
  return n;
}

void QRegister::applySingleGate(QID qID, Gate gate) {
  vectors[listIndexFromQID(qID)].applySingleGate(qID, gate);
}

void QRegister::applyCnGate(vector<QID> controls, QID qID, Gate gate) {
  list_index combinedIndex = 0;
  for(QID c : controls) combinedIndex = combineTwoQubits(qID, c);

  vectors[combinedIndex].applyCnGate(controls, qID, gate);
}

void QRegister::applyMultiGate(vector<QID> qIDs, Gate gate) {
  list_index combinedIndex = 0;
  for(QID c : qIDs) combinedIndex = combineTwoQubits(qIDs[0], c);
  
  vectors[combinedIndex].applyMultiGate(qIDs, gate);
}

void QRegister::flipQubit(QID qID) {
  applySingleGate(qID, X());
}

list_index QRegister::combineTwoQubits(QID q1ID, QID q2ID) {
  list_index q1Index = listIndexFromQID(q1ID);
  list_index q2Index = listIndexFromQID(q2ID);

  // if already combined, do nothing
  if(q1Index == q2Index) return q1Index;

  StateVector combinedState = vectors[q1Index].combineWith(vectors[q2Index]);
  unsigned int indexOfCombinedState = min(q1Index, q2Index);
  vectors[indexOfCombinedState] = combinedState;
  vectors.erase(vectors.begin() + max(q1Index, q2Index));
  return indexOfCombinedState;
}

list_index QRegister::listIndexFromQID(QID qID) {
  for(list_index i = 0; i < vectors.size(); i++) {
    QID* qubitIDs = vectors[i].qubitIDs();
    if (find(qubitIDs, qubitIDs+vectors[i].numberOfQubits(), qID) != qubitIDs+vectors[i].numberOfQubits()) {
      return i;
    }
  }

  throw exception();
}

string QRegister::measure() {
  vector<char> result (' ', n);

  for(StateVector v: vectors) {
    string measurement = v.measure();
    QID* qIDs = v.qubitIDs();
    for(int i = 0; i < measurement.length(); i++) {
      QID qID = qIDs[i];
      result[qID] = measurement[i];
    }
  }

  return string(result.begin(), result.end());
}

string QRegister::ketMeasure() {
  return "|" + measure() + ">";
}

vector<QID> QRegister::allQubits() {
  vector<QID> result;
  for (QID i = 0; i < n; i++) {
    result.push_back(i);
  }
  return result;
}

/**** CODE FOR A QUANTUM REGISTER ONLY (WITHOUT STATEVECTOR) IMPLEMENTATION ****/

//int nthCleared(int n, int t) {
//  int mask = (1 << t) - 1;
//  int notMask = ~mask;
//
//  return (n & mask) | ((n & notMask) << 1);
//}
//
//QRegister::QRegister(uint n) : n{n} {
//  amplitudes = new cxd[pow(2,n)];
//  amplitudes[0] = cxd(1);
//}
//
//void QRegister::applySingleGate(QID qID, cxd gate[4]) {
//#pragma omp parallel for
//  for(int i = 0; i < pow(2,n-1); i++) {
//    int zero_state = nthCleared(i, qID);
//    int one_state = zero_state | (1 << qID);
//
//    cxd zero_amp = amplitudes[zero_state];
//    cxd one_amp = amplitudes[one_state];
//
//    amplitudes[zero_state] = gate[0]*zero_amp + gate[1]*one_amp;
//    amplitudes[one_state] = gate[2]*zero_amp + gate[3]*one_amp;
//  }
//}
//
//void QRegister::applyCnGate(vector<QID> controls, QID qID, cxd gate[4]) {
//#pragma omp parallel for
//  for(int i = 0; i < pow(2,n-1); i++) {
//    int zero_state = nthCleared(i, qID);
//    int one_state = zero_state | (1 << qID);
//
//    cxd zero_amp = amplitudes[zero_state];
//    cxd one_amp = amplitudes[one_state];
//
//    bool control_zero = true;
//    bool control_one = true;
//
//    for(int c = 0; c < controls.size(); c++) {
//      if(! (((1 << controls[c]) & zero_state) > 0)) control_zero = false;
//      if(! (((1 << controls[c]) & one_state) > 0)) control_one = false;
//
//      if(!control_zero && !control_one) break;
//    }
//
//    if(control_zero) amplitudes[zero_state] = gate[0]*zero_amp + gate[1]*one_amp;
//    if(control_one) amplitudes[one_state] = gate[2]*zero_amp + gate[3]*one_amp;
//  }
//}
//
//vector<string> product(vector<string> v1, vector<string> v2) {
//  vector<string> results = {};
//
//  for(string i: v1)
//    for(string j: v2)
//      results.push_back(i + j);
//
//  return results;
//}
//
//vector<string> qubitStatesCombinations(unsigned int n) {
//  vector<string> results = {"0", "1"};
//
//  for(int i = 0; i < n-1; i++)
//    results = product(results, vector<string>({"0","1"}));
//
//  return results;
//}
//
//string QRegister::measure() {
//
//  vector<string> combs = qubitStatesCombinations(n);
//  vector<double> probs;
//
//  for(int i = 0; i < pow(2,n); i++) probs.push_back(norm(amplitudes[i]));
//
//  random_device rd;
//  mt19937 gen(rd());
//  discrete_distribution<> d (probs.begin(), probs.end());
//
//  return combs[d(gen)];
//}
//
//vector<QID> QRegister::allQubits() {
//  vector<QID> result;
//  for (QID i = 0; i < n; i++) {
//    result.push_back(i);
//  }
//  return result;
//}
