#include "Tensor.hpp"

// #include <iostream>
// #include <complex>
// #include <vector>
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

	#pragma omp parallel for num_threads(4)
	for(int i = 0; i < rowCount(); i++)
		#pragma omp parallel for num_threads(4)
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

	#pragma omp parallel for num_threads(4)
	for(int i = 0; i < result.rowCount(); i++) {
		#pragma omp parallel for num_threads(4)
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

	#pragma omp parallel for num_threads(4)
	for(int i = 0; i < result.rowCount(); i++)
		#pragma omp parallel for num_threads(4)
		for(int j = 0; j < result.colCount(); j++)
			result.setElementAt(i, j, elementAt(i/t1r, j/t1c) * t.elementAt(i%t2r, j%t2c));

	return result;
}

string Tensor::toString() {
	string result = "";

	for(int i = 0; i < rowCount(); i++) {
		if(i > 0) result += "\n";
		for(int j = 0; j < colCount(); j++) {
			cxd val = elementAt(i,j);
			result += to_string(val.real()) + (val.imag() != 0 ? " + " + to_string(val.imag()) + "i" : "") + "\t";
		}
	}

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

/************* Q Gates ***************/
Tensor I() {
	Tensor t (2,2);
	t.setElementAt(0,0,1);
	t.setElementAt(1,1,1);
	return t;
}

Tensor H() {
	Tensor t (2,2);
	t.setElementAt(0,0,1);
	t.setElementAt(0,1,1);
	t.setElementAt(1,0,1);
	t.setElementAt(1,1,-1);
	t = t.multiplyBy(1/sqrt(2));
	return t;
}
/************* END Q Gates ***************/

int main(int argc, char const *argv[]) {
	auto start = chrono::steady_clock::now();

	// omp_set_num_threads(10);

	// Tensor myTensor (2,1);
	// myTensor.setElementAt(0,0,cxd(1,0));
	
	Tensor identity = I();
	for(int i = 0; i < 12; i++) {
		cout << i << endl;
		identity = identity.kronWith(I());
	}


	// cout << H().kronWith(I()).toString();

	auto end = chrono::steady_clock::now();
	auto diff = end - start;
	cout << (chrono::duration <double, milli> (diff).count())/1000 << " s" << endl;
}



