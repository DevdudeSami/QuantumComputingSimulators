#include "Tensor.hpp"

#include <time.h>
#include <assert.h> 
#include <omp.h>

using namespace std;

Tensor::Tensor(Rows rs) {
	int firstRowSize = rs[0].size();
	for(Row row: rows) assert(row.size() == firstRowSize);
	rows = rs;
}
Tensor::Tensor(int rowCount, int colCount) {
	rows = Rows(rowCount, Row(colCount, cxd(0)));
}
Tensor::Tensor(Row v) {
  rows = Rows(1, v);
}

int Tensor::rowCount() {
	return rows.size();
}
int Tensor::colCount() {
	if(rowCount() != 0) return rows[0].size();
	return 0;
}
cxd Tensor::elementAt(int r, int c) {
	return rows[r][c];
}
void Tensor::setElementAt(int r, int c, cxd newValue) {
	rows[r][c] = newValue;
}
Row Tensor::getRow(int r) {
	return rows[r];
}
Row Tensor::getCol(int c) {
	Row result = Row(rowCount(), cxd(0));
	transform(rows.begin(), rows.end(), result.begin(), [c](Row row) { return row[c]; });
	return result;
}
Rows Tensor::getRows() {
  return rows;
}
Tensor Tensor::getBlock(int rs, int re, int cs, int ce) {
  assert(re > rs);
  assert(ce > cs);
  Tensor result (re-rs, ce-cs);
  
  #pragma omp parallel for num_threads(8)
  for(int i = rs; i < re; i++)
    #pragma omp parallel for num_threads(8)
    for(int j = cs; j < ce; j++)
      result.setElementAt(i-rs, j-cs, elementAt(i, j));
  
  return result;
}
void Tensor::setBlock(int rs, int re, int cs, int ce, Tensor t) {
  assert(re > rs);
  assert(ce > cs);
  assert(t.rowCount() == re-rs);
  assert(t.colCount() == ce-cs);
  
  #pragma omp parallel for num_threads(8)
  for(int i = rs; i < re; i++)
    #pragma omp parallel for num_threads(8)
    for(int j = cs; j < ce; j++)
      setElementAt(i, j, t.elementAt(i-rs, j-cs));
  
}
bool Tensor::matchesDimensionsWith(Tensor t) {
	return rowCount() == t.rowCount() && colCount() == t.colCount();
}

// TODO: Parallelise this possibly
cxd Tensor::dotProductWith(Tensor t) {
	assert(matchesDimensionsWith(t));

	cxd sum = 0;

	for(int i = 0; i < rowCount(); i++)
		for(int j = 0; j < colCount(); j++)
			sum += elementAt(i,j) * t.elementAt(i,j);

	return sum;
}

Tensor Tensor::multiplyBy(cxd k) {
	Tensor result (rowCount(), colCount());

	#pragma omp parallel for num_threads(8)
	for(int i = 0; i < rowCount(); i++)
		#pragma omp parallel for num_threads(8)
		for(int j = 0; j < colCount(); j++)
			result.setElementAt(i, j, elementAt(i,j) * k);

	return result;
}

Tensor Tensor::addTo(Tensor t) {
	assert(matchesDimensionsWith(t));
	Tensor result (rowCount(), colCount());

	#pragma omp parallel for num_threads(4)
	for(int i = 0; i < rowCount(); i++)
		#pragma omp parallel for num_threads(4)
		for(int j = 0; j < colCount(); j++)
			result.setElementAt(i, j, elementAt(i,j) + t.elementAt(i,j));

	return result;
}

Tensor Tensor::multiplyBy(Tensor t) {
	assert(colCount() == t.rowCount());
	Tensor result (rowCount(), t.colCount());

	#pragma omp parallel for num_threads(8)
	for(int i = 0; i < result.rowCount(); i++) {
		#pragma omp parallel for num_threads(8)
		for(int j = 0; j < result.colCount(); j++) {
			Row row = getRow(i);
			Row col = t.getCol(j);
			result.setElementAt(i, j, Tensor::vectorDotProduct(row,col));
		}
	}
			
	return result;
}

Tensor Tensor::kronWith(Tensor t) {
	int t1r = rowCount();
	int t1c = colCount();
	int t2r = t.rowCount();
	int t2c = t.colCount();

	Tensor result (t1r*t2r, t1c*t2c);
  
  #pragma omp parallel for num_threads(8)
  for(int i = 0; i < t1r; i++)
    #pragma omp parallel for num_threads(8)
    for(int j = 0; j < t1c; j++)
      result.setBlock(i*t2r, (i+1)*t2r, j*t2c, (j+1)*t2c, t.multiplyBy(elementAt(i, j)));
      
	return result;
}

string Tensor::toString() {
	string result = "";

	for(int i = 0; i < rowCount(); i++) {
		for(int j = 0; j < colCount(); j++) {
			cxd val = elementAt(i,j);
			result += to_string(val.real()) + (val.imag() != 0 ? " + " + to_string(val.imag()) + "i" : "") + "\t";
		}
    result += "\n";
	}

	return result;
}

void Tensor::enumerateElements(function<void(int,int,cxd)> f) {
  for(int i = 0; i < rowCount(); i++)
    for(int j = 0; j < colCount(); j++)
      f(i,j,elementAt(i, j));
}

bool Tensor::isNormalised() {
  float sumOfNorms = 0;
  
  enumerateElements([&sumOfNorms](int i, int r, cxd e) {
    sumOfNorms += abs(e);
  });
  
  return (int)sumOfNorms == 1;
}

Tensor Tensor::transpose() {
  Tensor result (colCount(), rowCount());
  
  for(int i = 0; i < rowCount(); i++)
    for(int j = 0; j < colCount(); j++)
      result.setElementAt(j, i, elementAt(i, j));
  
  return result;
}

/** Static Functions **/
cxd Tensor::vectorDotProduct(Row v1, Row v2) {
assert(v1.size() == v2.size());

cxd sum = 0;

for(int i = 0; i < v1.size(); i++)
	sum += v1[i] * v2[i];

return sum;
}

//int main(int argc, char const *argv[]) {
//  auto start = chrono::steady_clock::now();
//
//  // omp_set_num_threads(10);
//
//  // Tensor myTensor (2,1);
//  // myTensor.setElementAt(0,0,cxd(1,0));
//
//  Tensor identity = I();
//  for(int i = 0; i < 12; i++) {
//    cout << i << endl;
//    identity = identity.kronWith(I());
//  }
//
//
//  // cout << H().kronWith(I()).toString();
//
//  auto end = chrono::steady_clock::now();
//  auto diff = end - start;
//  cout << (chrono::duration <double, milli> (diff).count())/1000 << " s" << endl;
//}
//


