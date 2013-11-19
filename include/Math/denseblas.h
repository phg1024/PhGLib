#pragma once

#include "DenseMatrix.hpp"
#include "DenseVector.hpp"
#include <armadillo>

// use the linear solver in MKL
template <typename T>
DenseVector<T> solve(const DenseMatrix<T>& A, const DenseVector<T>& b) {
	arma::mat Amat;	
	arma::vec bvec;

	Amat.resize(A.rows(), A.cols());

	for(int i=0;i<A.cols();i++) {
		for(int j=0;j<A.rows();j++) {
			Amat(j, i) = A(j, i);
		}
	}

	Amat.print("A");

	bvec.resize(b.length());
	for(int i=0;i<b.length();i++) {
		bvec(i) = b(i);
	}

	arma::vec x = arma::solve(Amat, bvec);

	DenseVector<T> res(x.size());
	for(int i=0;i<x.size();i++) {
		res(i) = x(i);
	}

	res.print("res");
	return res;
}