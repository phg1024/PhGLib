#pragma once

#include "MatrixBase.hpp"
#include "DenseVector.hpp"

template <typename T>
class DenseMatrix : public MatrixBase<T>
{
public:
	typedef typename MatrixBase<T>::elem_t elem_t;
	typedef typename MatrixBase<T>::idx_t idx_t;

public:
	/// general constructors
	DenseMatrix();
	DenseMatrix(size_t numRow, size_t numCol);

	DenseMatrix(const DenseMatrix&);
	DenseMatrix& operator=(const DenseMatrix&);

	DenseMatrix(DenseMatrix&&);
	DenseMatrix& operator=(DenseMatrix&&);

	virtual ~DenseMatrix();

	/// special constructors
	static DenseMatrix zeros(size_t numRows, size_t numCols);
	static DenseMatrix ones(size_t numRows, size_t numCols);
	static DenseMatrix diag(size_t numRows, size_t numCols, T value, int offset);

	/// matrix operations
	DenseMatrix transposed();
	DenseMatrix tril(bool strict = false);
	DenseMatrix triu(bool strict = false);
	DenseMatrix diag();
	DenseMatrix inverted();

	/// arithmetic operations
	DenseMatrix operator+(const DenseMatrix&);
	DenseMatrix& operator+=(const DenseMatrix&);
	DenseMatrix operator-(const DenseMatrix&);
	DenseMatrix& operator-=(const DenseMatrix&);
	DenseMatrix operator*(double factor);
	DenseMatrix operator*(const DenseMatrix&);
	DenseMatrix operator*=(double factor);

	DenseVector<T> operator*(const DenseVector<T>&);
	template <typename MT>
	friend DenseMatrix<MT> operator*(double factor, DenseMatrix<MT>&);

	DenseMatrix operator/(double factor);
	DenseMatrix operator/=(double factor);

	/// element accessor
	virtual const T& operator()(idx_t i, idx_t j) const;
	virtual T& operator()(idx_t i, idx_t j);

	const T& operator()(idx_t idx) const;
	T& operator()(idx_t idx);

	virtual const size_t& rows() const;
	virtual const size_t& cols() const;

	/// misc operations
	virtual bool isValid() const;
	virtual void print(const string& title = "") const;
	void print(const string& title, bool trans) const;

	elem_t* ptr() {return mElems;}
	const elem_t* ptr() const {return mElems;}

private:
	size_t mRows, mCols;
	elem_t *mElems;
};

template <typename T>
DenseMatrix<T>::DenseMatrix():
	mRows(0),
	mCols(0),
	mElems(nullptr),
	MatrixBase<T>(MatrixBase<T>::Dense, MatrixBase<T>::ColumnMajor)
{}

template <typename T>
DenseMatrix<T>::DenseMatrix(size_t numRows, size_t numCols):
	mRows(numRows),
	mCols(numCols),
	mElems(nullptr),
	MatrixBase<T>(MatrixBase<T>::Dense, MatrixBase<T>::ColumnMajor)
{
	if( numRows * numCols > 0 )
	{
		mElems = ArrayAllocator<T>::instance().allocate(numRows * numCols);
		memset(mElems, 0, sizeof(T)*numRows*numCols);
	}
}

template <typename T>
DenseMatrix<T>::DenseMatrix(const DenseMatrix& other):
	mRows(other.mRows),
	mCols(other.mCols),
	mElems(ArrayAllocator<T>::instance().allocateother.mRows * other.mCols),
	MatrixBase<T>(MatrixBase<T>::Dense, MatrixBase<T>::ColumnMajor)
{
	memcpy(mElems, other.mElems, sizeof(T)*other.mRows*other.mCols);
}

template <typename T>
DenseMatrix<T>& DenseMatrix<T>::operator=(const DenseMatrix& other)
{
	if (this != &other)
	{
		// Free the existing resource.
		if( mElems != nullptr )
			delete[] mElems;

		mRows = other.mRows;
		mCols = other.mCols;
		mElems = ArrayAllocator<T>::instance().allocate(mRows * mCols);

		mType = other.mType;
		mFormat = other.mFormat;

		memcpy(mElems, other.mElems, sizeof(T)*other.mRows*other.mCols);
	}
	return *this;
}

template <typename T>
DenseMatrix<T>::DenseMatrix(DenseMatrix&& other):
	mRows(0),
	mCols(0),
	mElems(nullptr),
	MatrixBase<T>(MatrixBase<T>::Dense, MatrixBase<T>::ColumnMajor)
{
	mRows = other.mRows;
	mCols = other.mCols;
	mElems = other.mElems;

	other.mElems = nullptr;
	other.mRows = 0;
	other.mCols = 0;
}

template <typename T>
DenseMatrix<T>& DenseMatrix<T>::operator=(DenseMatrix&& other)
{
	if (this != &other)
	{
		mRows = other.mRows;
		mCols = other.mCols;
		mElems = other.mElems;

		mType = other.mType;
		mFormat = other.mFormat;

		other.mElems = nullptr;
		other.mRows = 0;
		other.mCols = 0;
	}
	return *this;
}

template <typename T>
DenseMatrix<T>::~DenseMatrix() {
	ArrayAllocator<T>::instance().release(mElems, mRows*mCols);
}

template <typename T>
DenseMatrix<T> DenseMatrix<T>::zeros(size_t numRows, size_t numCols)
{
	return DenseMatrix(numRows, numCols);
}

template <typename T>
DenseMatrix<T> DenseMatrix<T>::ones(size_t numRows, size_t numCols)
{
	DenseMatrix mat(numRows, numCols);

	for(size_t i=0;i<numCols;i++)
	{
		for(size_t j=0;j<numRows;j++)
			mat(i, j) = 1;
	}

	return mat;
}

/// @bug NOT CORRECTLY IMPLEMENTED
template <typename T>
DenseMatrix<T> DenseMatrix<T>::diag(size_t numRows, size_t numCols, T value, int offset)
{
	DenseMatrix mat(numRows, numCols);

	int size = (numRows < numCols)?numRows:numCols;

	if( offset >= 0 )
	{
		for(size_t i=0;i<numRows;i++)
			if( i + offset < numCols )
				mat(i, i+offset) = value;
	}
	else
	{
		for(size_t i=-offset;i<numRows;i++)
		{
			if( i + offset < numCols )
				mat(i, i+offset) = value;
		}
	}

	return mat;
}

template <typename T>
DenseMatrix<T> DenseMatrix<T>::operator +(const DenseMatrix<T> &rhs)
{
	// check if the dimension matches
	if( !isValid() || !rhs.isValid() )
	{
		Utility::fail("DenseMatrix::operator* : input is invalid!");
		return DenseMatrix<T>();
	}
	else if( rhs.rows() != mRows || rhs.cols() != mCols )
	{
		Utility::fail("DenseMatrix::operator* : input dimensions do not match");
		return DenseMatrix<T>();
	}
	else
	{
		DenseMatrix<T> result(mRows, rhs.cols());

		LS_BLAS::dgeadd(mRows, mCols,
			1.0,
			mElems, rhs.ptr(),
			result.ptr());

		return result;
	}
}

template <typename T>
DenseMatrix<T> DenseMatrix<T>::operator -(const DenseMatrix<T> &rhs)
{
	// check if the dimension matches
	if( !isValid() || !rhs.isValid() )
	{
		Utility::fail("DenseMatrix::operator* : input is invalid!");
		return DenseMatrix<T>();
	}
	else if( rhs.rows() != mRows || rhs.cols() != mCols )
	{
		Utility::fail("DenseMatrix::operator* : input dimensions do not match");
		return DenseMatrix<T>();
	}
	else
	{
		DenseMatrix<T> result(mRows, rhs.cols());

		LS_BLAS::dgeadd(mRows, mCols,
			-1.0,
			mElems, rhs.ptr(),
			result.ptr());

		return result;
	}
}

template <typename T>
DenseMatrix<T> DenseMatrix<T>::operator *(const DenseMatrix<T> &rhs)
{
	// check if the dimension matches
	if( !isValid() || !rhs.isValid() )
	{
		Utility::fail("DenseMatrix::operator* : input is invalid!");
		return DenseMatrix<T>();
	}
	else if( rhs.rows() != mCols )
	{
		Utility::fail("DenseMatrix::operator* : input dimensions do not match");
		return DenseMatrix<T>();
	}
	else
	{
		DenseMatrix<T> result(mRows, rhs.cols());

		LS_BLAS::dgemm(mRows, rhs.cols(), mCols,
			1.0, 0,
			mElems, rhs.ptr(),
			result.ptr());

		return result;
	}
}

template <typename T>
const T& DenseMatrix<T>::operator()(idx_t i, idx_t j) const
{
	return mElems[j*mRows + i];
}

template <typename T>
T& DenseMatrix<T>::operator()(idx_t i, idx_t j)
{
	return mElems[j*mRows + i];
}

template <typename T>
const T& DenseMatrix<T>::operator()(idx_t idx) const
{
	return mElems[idx];
}

template <typename T>
T& DenseMatrix<T>::operator()(idx_t idx)
{
	return mElems[idx];
}

template <typename T>
const size_t& DenseMatrix<T>::rows() const
{
	return mRows;
}

template <typename T>
const size_t& DenseMatrix<T>::cols() const
{
	return mCols;
}

template <typename T>
bool DenseMatrix<T>::isValid() const
{
	return (mRows > 0) && (mCols > 0) && (mElems != nullptr);
}

template <typename T>
void DenseMatrix<T>::print(const string& title) const
{
	if( title != "" )
		std::cout << title << " = " << std::endl;

	if( !isValid() )
	{
		cerr << "Not a valid matrix!" << endl;
		return;
	}

	for(size_t i=0;i<mRows;i++)
	{
		for(size_t j=0;j<mCols;j++)
			std::cout << (*this)(i, j) << "\t";

		std::cout << std::endl;
	}
}

template <typename T>
void DenseMatrix<T>::print(const string& title, bool trans) const
{
	if( title != "" )
		std::cout << title << " = " << std::endl;

	if( !isValid() )
	{
		cerr << "Not a valid matrix!" << endl;
		return;
	}

	if( trans )
	{
		for(size_t i=0;i<mRows;i++)
		{
			for(size_t j=0;j<mCols;j++)
				std::cout << mElems[i * mCols + j] << "\t";

			std::cout << std::endl;
		}
	}
	else
	{
		for(size_t i=0;i<mRows;i++)
		{
			for(size_t j=0;j<mCols;j++)
				std::cout << (*this)(i, j) << "\t";

			std::cout << std::endl;
		}
	}
}

typedef DenseMatrix<float> DenseMatrixf;
typedef DenseMatrix<double> DenseMatrixd;