#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "fftn.h"

void test1() {
	double Re[3][2] = { {1.0, 0.0}, {2.0,-1.0}, {3.0, -4.0} };
	double Im[3][2] = { {4.0, -1.0}, {0.0,1.0}, {2.0, 1.0} };
	int dims[2] = {2, 3};

	for (int row=0;row<2; ++row) {
		for (int col=0;col<3; ++col) {
			printf("%7.2f + %7.2f ;  ", Re[col][row], Im[col][row]);
		}
		printf("\n");
	}
	printf("\n");

	fftn<double> (2, dims, (double*)Re, (double*)Im, 1, -1);

	for (int row=0;row<2; ++row) {
		for (int col=0;col<3; ++col) {
			printf("%7.2f + %7.2f ;  ", Re[col][row], Im[col][row]);
		}
		printf("\n");
	}
	printf("\n");

	fftn<double> (2, dims, (double*)Re, (double*)Im, -1, 1);

	for (int row=0;row<2; ++row) {
		for (int col=0;col<3; ++col) {
			printf("%7.2f + %7.2f ;  ", Re[col][row], Im[col][row]);
		}
		printf("\n");
	}
	printf("\n");
}


int main(int argc, const char * const argv[])
{
	const unsigned int W=9, H=9;
	int dims[2] = {H, W};
	double Re[W][H]; memset(Re, 0, sizeof(Re));
	double Im[W][H]; memset(Im, 0, sizeof(Im));
	Re[W/2][H/2] = 1.0;

	for (int row=0;row<dims[1]; ++row) {
		for (int col=0;col<dims[0]; ++col) {
			printf("%5.1f + %5.1f ;  ", Re[col][row], Im[col][row]);
		}
		printf("\n");
	}
	printf("\n");

	fftn<double> (2, dims, (double*)Re, (double*)Im, 1, -1); // Direct FFT

	for (int row=0;row<dims[1]; ++row) {
		for (int col=0;col<dims[0]; ++col) {
			printf("%5.1f + %5.1f ;  ", Re[col][row], Im[col][row]);
		}
		printf("\n");
	}
	printf("\n");

// If we want the result to be a matrix of real numbers, the original matrix
// must verify the following conditions :
//                 ______              ______
//    A(N-i,N-j) = A(i,j) , A(0,N-j) = A(0,j)
//
//    A(N-i,0) = A(i,0) for i,j > 0, and A(0,0) is a pure real


	fftn<double> (2, dims, (double*)Re, (double*)Im, -1, 1); // Inverse FFT

	for (int row=0;row<dims[1]; ++row) {
		for (int col=0;col<dims[0]; ++col) {
			printf("%5.1f + %5.1f ;  ", Re[col][row], Im[col][row]);
		}
		printf("\n");
	}
	printf("\n");

	return 0;
}

