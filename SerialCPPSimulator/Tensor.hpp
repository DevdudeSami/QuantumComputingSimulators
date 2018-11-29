#include <iostream>
#include <complex>
#include <vector>

using cxd = std::complex<double>;
using Row = std::vector<cxd>;
using Rows = std::vector<Row>;

class Tensor {
	private:
		Rows rows;
	public:
		Tensor(Rows rs);
		Tensor(int rowCount, int colCount);
		int rowCount();
		int colCount();
		cxd elementAt(int r, int c);
		void setElementAt(int r, int c, cxd newValue);
		Row getRow(int r);
		Row getCol(int c);
		bool matchesDimensionsWith(Tensor t);

		cxd dotProductWith(Tensor t);
		Tensor multiplyBy(cxd k);
		Tensor addTo(Tensor t);

		Tensor multiplyBy(Tensor t);

		Tensor kronWith(Tensor t);

		std::string toString();

	/** Static Functions **/
	static cxd vectorDotProduct(Row v1, Row v2);
};