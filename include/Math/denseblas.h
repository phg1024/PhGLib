#pragma once

#include "DenseMatrix.hpp"
#include "DenseVector.hpp"
#include <armadillo>
#include "mkl.h"

template <typename T, typename MT>
MT toMat(const DenseMatrix<T>& A) {
	MT Amat(A.rows(), A.cols());

	for(int i=0;i<A.cols();i++) {
		for(int j=0;j<A.rows();j++) {
			Amat(j, i) = A(j, i);
		}
	}
	return Amat;
}

template <typename T>
lapack_int xgels(int, lapack_int, lapack_int, lapack_int, T*, 
		lapack_int, T*, lapack_int, T*, T, lapack_int*) 
{}

template <> lapack_int xgels<float>(int order, lapack_int m, lapack_int n, lapack_int nrhs, 
							  float* a, lapack_int lda, 
							  float* b, lapack_int ldb, 
							  float* s, float rcond, lapack_int* rank) 
{
	return LAPACKE_sgelsd(order, m, n, nrhs, a, lda, b, ldb, s, rcond, rank);
}

template <> lapack_int xgels<double>(int order, lapack_int m, lapack_int n, lapack_int nrhs, 
							  double* a, lapack_int lda, 
							  double* b, lapack_int ldb, 
							  double* s, double rcond, lapack_int* rank) 
{
	return LAPACKE_dgelsd(order, m, n, nrhs, a, lda, b, ldb, s, rcond, rank);
}

template <typename T>
arma::vec toVec(const DenseVector<T>& b) {
	arma::vec bvec(b.length());
	for(int i=0;i<b.length();i++) {
		bvec(i) = b(i);
	}
	return bvec;
}

// least square solver
template <typename T>
lapack_int leastsquare(DenseMatrix<T>& A, DenseVector<T>& b) {
	MKL_INT rank;
	lapack_int m = A.rows(), n = A.cols();
	DenseVector<T> s(m);
	// call the wrapper
	return xgels<T>(LAPACK_COL_MAJOR, m, n, 1, A.ptr(), A.rows(), b.ptr(), b.length(), 
		s.ptr(), -1.0, &rank);
}

template <typename T>
DenseVector<T> solve(const DenseMatrix<T>& A, const DenseVector<T>& b) {
	arma::mat Amat = toMat(A);
	arma::vec bvec = toVec(b);
	
	arma::vec x = arma::solve(Amat, bvec);

	DenseVector<T> res(x.size());
	for(int i=0;i<x.size();i++) {
		res(i) = x(i);
	}

	//res.print("res");
	return res;
}