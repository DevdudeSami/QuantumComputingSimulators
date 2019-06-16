#include "DenseTensor.hpp"

#include <time.h>
#include <assert.h> 
#include <omp.h>

using namespace std;

DenseTensor::DenseTensor(Rows rs) {
	int firstRowSize = rs[0].size();
	for(Row row: rows) assert(row.size() == firstRowSize);
	rows = rs;
}
DenseTensor::DenseTensor(int rowCount, int colCount) {
	rows = Rows(rowCount, Row(colCount, cxd(0)));
}
DenseTensor::DenseTensor(Row v) {
  rows = Rows(1, v);
}

uint DenseTensor::rowCount() {
	return rows.size();
}
uint DenseTensor::colCount() {
	if(rowCount() != 0) return rows[0].size();
	return 0;
}
uint DenseTensor::getNNZ() {
  uint nnz = 0;

  #pragma omp parallel for reduction (+:nnz)
  for(int i = 0; i < rowCount(); i++)
    #pragma omp parallel for reduction (+:nnz)
    for(int j = 0; j < colCount(); j++)
      if(elementAt(i, j) != cxd(0)) nnz++;
  
  return nnz;
}

cxd DenseTensor::elementAt(int r, int c) {
	return rows[r][c];
}
void DenseTensor::setElementAt(int r, int c, cxd newValue) {
	rows[r][c] = newValue;
}
Row DenseTensor::getRow(int r) {
	return rows[r];
}
Row DenseTensor::getCol(int c) {
	Row result = Row(rowCount(), cxd(0));
	transform(rows.begin(), rows.end(), result.begin(), [c](Row row) { return row[c]; });
	return result;
}
Rows DenseTensor::getRows() {
  return rows;
}
DenseTensor DenseTensor::getBlock(int rs, int re, int cs, int ce) {
  assert(re > rs);
  assert(ce > cs);
  DenseTensor result (re-rs, ce-cs);
  
  #pragma omp parallel for
  for(int i = rs; i < re; i++)
    #pragma omp parallel for
    for(int j = cs; j < ce; j++)
      result.setElementAt(i-rs, j-cs, elementAt(i, j));
  
  return result;
}
void DenseTensor::setBlock(int rs, int re, int cs, int ce, DenseTensor t) {
  assert(re > rs);
  assert(ce > cs);
  assert(t.rowCount() == re-rs);
  assert(t.colCount() == ce-cs);
  
  #pragma omp parallel for
  for(int i = rs; i < re; i++)
    #pragma omp parallel for
    for(int j = cs; j < ce; j++)
      setElementAt(i, j, t.elementAt(i-rs, j-cs));
  
}
bool DenseTensor::matchesDimensionsWith(DenseTensor t) {
	return rowCount() == t.rowCount() && colCount() == t.colCount();
}

// TODO: Parallelise this possibly
cxd DenseTensor::dotProductWith(DenseTensor t) {
	assert(matchesDimensionsWith(t));

	cxd sum = 0;

	for(int i = 0; i < rowCount(); i++)
		for(int j = 0; j < colCount(); j++)
			sum += elementAt(i,j) * t.elementAt(i,j);

	return sum;
}

DenseTensor DenseTensor::multiplyBy(cxd k) {
	DenseTensor result (rowCount(), colCount());

	#pragma omp parallel for
	for(int i = 0; i < rowCount(); i++)
		#pragma omp parallel for
		for(int j = 0; j < colCount(); j++)
			result.setElementAt(i, j, elementAt(i,j) * k);

	return result;
}

DenseTensor DenseTensor::addTo(DenseTensor t) {
	assert(matchesDimensionsWith(t));
	DenseTensor result (rowCount(), colCount());

	#pragma omp parallel for
	for(int i = 0; i < rowCount(); i++)
		#pragma omp parallel for
		for(int j = 0; j < colCount(); j++)
			result.setElementAt(i, j, elementAt(i,j) + t.elementAt(i,j));

	return result;
}

DenseTensor DenseTensor::multiplyTo(DenseTensor t) {
	assert(colCount() == t.rowCount());
	DenseTensor result (rowCount(), t.colCount());

	#pragma omp parallel for
	for(int i = 0; i < result.rowCount(); i++) {
		#pragma omp parallel for
		for(int j = 0; j < result.colCount(); j++) {
			Row row = getRow(i);
			Row col = t.getCol(j);
			result.setElementAt(i, j, DenseTensor::vectorDotProduct(row,col));
		}
	}
			
	return result;
}

SparseTensor DenseTensor::multiplyTo(SparseTensor t) {
  assert(colCount() == t.rowCount());
  assert(t.colCount() == 1);
  
  // The best way to do this is to probably convert t to a dense matrix and do the multiplication that way.
  // Especially that t.dense() is parallelised.
  
  /** This is the same algorithm as the above multiplyBy function
      but with keeping an nnz counter and optimised for multiplying by a vector. **/
  DenseTensor result (rowCount(), 1);
  Row col = t.dense().getCol(0);
  uint nnz = 0;
  
  #pragma omp parallel for
  for(int i = 0; i < result.rowCount(); i++) {
    Row row = getRow(i);
    cxd dotProduct = DenseTensor::vectorDotProduct(row,col);
    if(dotProduct != cxd(0)) {
      nnz++;
      result.setElementAt(i, 0, dotProduct);
    }
  }
  /** end algorithm **/
  
  return SparseTensor(result, nnz);
}

DenseTensor DenseTensor::denseKronWith(DenseTensor t) {
	int t1r = rowCount();
	int t1c = colCount();
	int t2r = t.rowCount();
	int t2c = t.colCount();

	DenseTensor result (t1r*t2r, t1c*t2c);
  
  // TODO: Optimise this; this can probably be done in two loops without using setBlock
  #pragma omp parallel for
  for(int i = 0; i < t1r; i++)
    #pragma omp parallel for
    for(int j = 0; j < t1c; j++)
      result.setBlock(i*t2r, (i+1)*t2r, j*t2c, (j+1)*t2c, t.multiplyBy(elementAt(i, j)));
      
	return result;
}

// TODO: Optimise this
DenseTensor DenseTensor::denseKronWith(SparseTensor t) {
  return denseKronWith(t.dense());
}

DenseTensor DenseTensor::denseKronWith(Tensor* t) {
  SparseTensor *asSparse = dynamic_cast<SparseTensor*>(t);
  if(asSparse != nullptr) return denseKronWith(*asSparse);
  else return denseKronWith(*dynamic_cast<DenseTensor*>(t));
}

// TODO: Optimise this
SparseTensor DenseTensor::sparseKronWith(DenseTensor t) {
  DenseTensor denseResult = denseKronWith(t);
  return SparseTensor(denseResult, denseResult.getNNZ());
}

// TODO: Optimise this
SparseTensor DenseTensor::sparseKronWith(SparseTensor t) {
  return sparseKronWith(t.dense());
}

SparseTensor DenseTensor::sparseKronWith(Tensor* t) {
  SparseTensor *asSparse = dynamic_cast<SparseTensor*>(t);
  if(asSparse != nullptr) return sparseKronWith(*asSparse);
  else return sparseKronWith(*dynamic_cast<DenseTensor*>(t));
}

string DenseTensor::toString() {
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

void DenseTensor::enumerateElements(function<void(int,int,cxd)> f) {
  for(int i = 0; i < rowCount(); i++)
    for(int j = 0; j < colCount(); j++)
      f(i,j,elementAt(i, j));
}

bool DenseTensor::isNormalised() {
  float sumOfNorms = 0;
  
  enumerateElements([&sumOfNorms](int i, int r, cxd e) {
    sumOfNorms += abs(e);
  });
  
  return (int)sumOfNorms == 1;
}

DenseTensor DenseTensor::transpose() {
  DenseTensor result (colCount(), rowCount());
  
  for(int i = 0; i < rowCount(); i++)
    for(int j = 0; j < colCount(); j++)
      result.setElementAt(j, i, elementAt(i, j));
  
  return result;
}

/** Static Functions **/
cxd DenseTensor::vectorDotProduct(Row v1, Row v2) {
  assert(v1.size() == v2.size());

  cxd sum = 0;

  #pragma omp parallel for reduction (+:sum)
  for(int i = 0; i < v1.size(); i++)
    sum += v1[i] * v2[i];

  return sum;
}


