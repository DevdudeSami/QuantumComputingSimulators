#include <iostream>
#include <armadillo>
#include <complex>
#include <vector>
#include <time.h> 

using namespace std;
using namespace arma;

class StateVector {
	private:
		int n;
		Col< cx_double > amplitudes;
		vector<int> registerIndices;
	public:
		StateVector(Col< cx_double > amps, vector<int> regIndices) {
			n = regIndices.size();
			amplitudes = amps;
			registerIndices = regIndices;
		}

		vector<double> probabilities() {
			vector<double> result;
			amplitudes.for_each([&result](cx_double amp) { result.push_back(norm(amp)); } );
			return result;
		}

};

int main(int argc, char const *argv[]) {
	
	clock_t t = clock();

	// cout << "Hello, World!" << endl;

	// double real[2] = {1, 0};
	// double imag[2] = {0, 0};

	// int indices[1] = {0};

	// vec real_vector (vector<double>(real, real + 2));
	// vec imag_vector (vector<double>(imag, imag + 2));

	// StateVector state (cx_vec(real_vector, imag_vector), vector<int>(indices, indices + 1));
	
	// vector<double> probs = state.probabilities();
	// for(int i = 0; i < probs.size(); i++ )
  //   cout << probs[i] << endl;

	Mat<double> A = arma::randu(64,64);

	// Mat<double> myArrayOfMats[47635];
	Mat<double> myArrayOfMats[47000];

	int chunk = 10;

	// #pragma omp parallel for default(shared) private(i) schedule(static, chunk)
	for(int i = 0; i < 47000; i++) {
		if(i%100 == 0) cout << i << endl;
		myArrayOfMats[i] = A*A;
	}

	t = clock() - t;
	printf("%f seconds.\n",((float)t)/CLOCKS_PER_SEC);

	return 0;
}