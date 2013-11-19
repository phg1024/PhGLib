#pragma once

#include "common.h"

template <typename T>
class VectorBase {
public:
    typedef T elem_t;
    typedef idx_t idx_t;

    enum VectorType
    {
        Dense = 0x0,
        Sparse = 0x1
    };

    enum VectorOrientation
    {
        Column = 0,
        Row = 1
    };

    VectorBase( VectorType type ):
        mType(type)
    {
        mElemSize = sizeof(T);
    }

    virtual ~VectorBase(){}

    virtual const T& operator() (idx_t i) const = 0;
    virtual T& operator() (idx_t i) = 0;
    virtual size_t length() const = 0;

    virtual bool isValid() const = 0;
    virtual void print(const string& title = "") const = 0;

    VectorType getType() const { return mType; }
    int getElementSize() const { return mElemSize; }

protected:
    VectorType mType;
    int mElemSize;
};