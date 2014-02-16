#pragma once

#include "../phgutils.h"
#include "../IO/arrayallocator.h"

template <typename T=double>
class MatrixBase {
public:
	typedef T elem_t;
	typedef idx_t idx_t;

	enum MatrixType
	{
		Dense = 0x0,
		Sparse = 0x2
	};

	enum MatrixFormat
	{
		ColumnMajor = 0,        // for dense matrix
		CSC,                // compressed sparse column
		CSR,                // compressed sparse row
		COO                 // coordinates format
	};

	MatrixBase( MatrixType type, MatrixFormat format):
		mType(type),
		mFormat(format)
	{
		mElemSize = sizeof(T);
	}

	virtual ~MatrixBase(){}

	virtual const T& operator() (idx_t i, idx_t j) const = 0;
	virtual T& operator() (idx_t i, idx_t j) = 0;
	virtual const size_t& rows() const = 0;
	virtual const size_t& cols() const = 0;

	virtual bool isSymmetric() const { return mSymmetric; }
	virtual bool& isSymmetric() { return mSymmetric; }
	virtual bool isValid() const = 0;
	virtual void print(const string& title = "", ostream& os = std::cout) const = 0;

	MatrixType getType() const { return mType; }
	MatrixFormat getFormat() const { return mFormat; }
	int getElementSize() const { return mElemSize; }

protected:
	MatrixType mType;
	MatrixFormat mFormat;
	int mElemSize;
	bool mSymmetric;
};