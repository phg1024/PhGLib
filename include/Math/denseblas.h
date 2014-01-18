#pragma once

#include "DenseMatrix.hpp"
#include "DenseVector.hpp"
#include <armadillo>
#include "mkl.h"
#include "cula.h"
#include "cula_lapack.h"

template <typename T, typename MT>
MT toMat(const PhGUtils::DenseMatrix<T>& A) {
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

static void culaCheckStatus(culaStatus status)
{
    char buf[256];

    if(!status)
        return;

    culaGetErrorInfoString(status, culaGetErrorInfo(), buf, sizeof(buf));
    printf("%s\n", buf);

    culaShutdown();
	system("pause");
    exit(EXIT_FAILURE);
}

template <> lapack_int xgels<float>(int order, lapack_int m, lapack_int n, lapack_int nrhs, 
							  float* a, lapack_int lda, 
							  float* b, lapack_int ldb, 
							  float* s, float rcond, lapack_int* rank) 
{
#if 1
	return LAPACKE_sgelsd(order, m, n, nrhs, a, lda, b, ldb, s, rcond, rank);
#else	
	culaStatus stas = culaSgels('N', m, n, nrhs, a, lda, b, ldb);
	culaCheckStatus(stas);
	return lapack_int(1);
#endif
}

template <> lapack_int xgels<double>(int order, lapack_int m, lapack_int n, lapack_int nrhs, 
							  double* a, lapack_int lda, 
							  double* b, lapack_int ldb, 
							  double* s, double rcond, lapack_int* rank) 
{
	return LAPACKE_dgelsd(order, m, n, nrhs, a, lda, b, ldb, s, rcond, rank);
}

template <typename T>
arma::vec toVec(const PhGUtils::DenseVector<T>& b) {
	arma::vec bvec(b.length());
	for(int i=0;i<b.length();i++) {
		bvec(i) = b(i);
	}
	return bvec;
}

// least square solver
template <typename T>
lapack_int leastsquare(PhGUtils::DenseMatrix<T>& A, PhGUtils::DenseVector<T>& b) {
	MKL_INT rank;
	lapack_int m = A.rows(), n = A.cols();
	PhGUtils::DenseVector<T> s(m);
	// call the wrapper
	return xgels<T>(LAPACK_COL_MAJOR, m, n, 1, A.ptr(), m, b.ptr(), m, 
		s.ptr(), -1.0, &rank);
}

// least square solver using normal equation
lapack_int leastsquare_normalmat(PhGUtils::DenseMatrix<float>& A, PhGUtils::DenseVector<float>& b, 
								 PhGUtils::DenseMatrix<float>& AtA, PhGUtils::DenseVector<float>& Atb) {
	lapack_int m = A.rows(), n = A.cols();
	// compute AtA
	cblas_ssyrk (CblasRowMajor, CblasUpper, CblasNoTrans, n, m, 1.0, A.ptr(), m, 0, AtA.ptr(), n);

	// compute Atb
	cblas_sgemv (CblasRowMajor, CblasNoTrans, n, m, 1.0, A.ptr(), m, b.ptr(), 1, 0, Atb.ptr(), 1);

	// compute AtA\Atb, since AtA is only semi-positive definite, we need to use LU-decomposition
#if 0
	PhGUtils::DenseVector<int> ipiv(n);
	LAPACKE_ssytrf( LAPACK_COL_MAJOR, 'L', n, AtA.ptr(), n, ipiv.ptr() );
	return LAPACKE_ssytrs(LAPACK_COL_MAJOR, 'L', n, 1, AtA.ptr(), n, ipiv.ptr(), Atb.ptr(), n);
#else
	LAPACKE_spotrf( LAPACK_COL_MAJOR, 'L', n, AtA.ptr(), n );
	return LAPACKE_spotrs( LAPACK_COL_MAJOR, 'L', n, 1, AtA.ptr(), n, Atb.ptr(), n );
#endif
}

template <typename T>
PhGUtils::DenseVector<T> solve(const PhGUtils::DenseMatrix<T>& A, const PhGUtils::DenseVector<T>& b) {
	arma::mat Amat = toMat(A);
	arma::vec bvec = toVec(b);
	
	arma::vec x = arma::solve(Amat, bvec);

	PhGUtils::DenseVector<T> res(x.size());
	for(int i=0;i<x.size();i++) {
		res(i) = x(i);
	}

	//res.print("res");
	return res;
}