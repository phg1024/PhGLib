#pragma once

#include "VectorBase.hpp"
#include <fstream>

template <typename T>
class DenseVector : public VectorBase<T>
{
public:
    typedef typename VectorBase<T>::idx_t idx_t;
    typedef typename VectorBase<T>::elem_t elem_t;

    DenseVector();
    DenseVector(size_t length);
    DenseVector(const elem_t* elems, size_t length);
    DenseVector(const DenseVector&);
    DenseVector& operator=(const DenseVector&);
    DenseVector(DenseVector&&);
    DenseVector& operator=(DenseVector&&);
    virtual ~DenseVector();

    DenseVector operator+(const DenseVector&);
    DenseVector operator+=(const DenseVector&);
    DenseVector operator-(const DenseVector&);
    DenseVector operator-=(const DenseVector&);
    DenseVector operator*(double);
    DenseVector operator*=(double);
    DenseVector operator/(double);

	elem_t dot(const DenseVector& v) const;
	elem_t sum() const;

    elem_t* ptr() { return mElems; }
    const elem_t* ptr() const { return mElems; }

    virtual const T& operator() (idx_t i) const;
    virtual T& operator() (idx_t i);
    virtual size_t length() const;

    virtual bool isValid() const;
    virtual void print(const string& title = "") const;

    virtual void save(const string& filename, size_t lineSize = 16) const;

private:
    size_t mLength;
    elem_t *mElems;
};

template <typename T>
DenseVector<T>::DenseVector():
    mLength(0),
    mElems(nullptr),
    VectorBase<T>(VectorBase<T>::Dense)
{}

template <typename T>
DenseVector<T>::DenseVector(size_t length):
    mLength(length),
    mElems(nullptr),
    VectorBase<T>(VectorBase<T>::Dense)
{
    mElems = ArrayAllocator<elem_t>::instance().allocate(length);
    memset(mElems, 0, sizeof(elem_t) * length);
}

template <typename T>
DenseVector<T>::DenseVector(const elem_t *elems, size_t length):
    mLength(length),
    mElems(ArrayAllocator<elem_t>::instance().allocate(length)),
    VectorBase<T>(VectorBase<T>::Dense)
{
    memcpy(mElems, elems, sizeof(elem_t) * length);
}

template <typename T>
DenseVector<T>::DenseVector(const DenseVector<T> &other):
    mLength(other.mLength),
    mElems(ArrayAllocator<elem_t>::instance().allocate(other.mLength)),
    VectorBase<T>(VectorBase<T>::Dense)
{
    memcpy(mElems, other.mElems, sizeof(elem_t) * other.length());
}

template <typename T>
DenseVector<T>& DenseVector<T>::operator=(const DenseVector<T>& other)
{
    if( this != &other )
    {
        if( mElems != nullptr )
            ArrayAllocator<elem_t>::instance().release(mElems, mLength);

        mLength = other.mLength;
        mElems = ArrayAllocator<elem_t>::instance().allocate(other.mLength);

        this->mType = other.mType;
        this->mElemSize = other.mElemSize;

        memcpy(mElems, other.mElems, sizeof(elem_t) * other.mLength);
    }

    return (*this);
}

template <typename T>
DenseVector<T>::DenseVector(DenseVector<T> &&other):
    mLength(0),
    mElems(nullptr),
    VectorBase<T>(VectorBase<T>::Dense)
{
    mLength = other.mLength;
    mElems = other.mElems;

    this->mType = other.mType;
    this->mElemSize = other.mElemSize;

    other.mElems = nullptr;
    other.mLength = 0;
}

template <typename T>
DenseVector<T>& DenseVector<T>::operator=(DenseVector<T>&& other)
{
    if( this != &other )
    {
        if( mElems != nullptr )
            ArrayAllocator<elem_t>::instance().release(mElems, mLength);

        mLength = other.mLength;
        mElems = other.mElems;

        this->mType = other.mType;
        this->mElemSize = other.mElemSize;

        other.mElems = nullptr;
        other.mLength = 0;
    }

    return (*this);
}

template <typename T>
DenseVector<T>::~DenseVector()
{
    ArrayAllocator<elem_t>::instance().release(mElems, mLength);
}


template <typename T>
DenseVector<T> DenseVector<T>::operator-(const DenseVector &rhs)
{
    if( rhs.length() != mLength )
    {
        cerr << "lengths do not match!" << endl;
        return DenseVector<T>();
    }
    else
    {
        DenseVector<T> res((*this));

        for(size_t i=0;i<res.length();i++)
        {
            res(i) -= rhs(i);
        }

        return res;
    }
}

template <typename T>
T DenseVector<T>::dot(const DenseVector &rhs) const
{
    if( rhs.length() != mLength )
    {
        cerr << "lengths do not match!" << endl;
        return 0;
    }
    else
    {
        T res = 0;

        for(size_t i=0;i<rhs.length();i++)
        {
            res += mElems[i] * rhs(i);
        }

        return res;
    }
}

template <typename T>
T DenseVector<T>::sum() const
{
	T res = 0;

	for(size_t i=0;i<mLength;i++)
	{
		res += mElems[i];
	}

	return res;
}

template <typename T>
const T& DenseVector<T>::operator() (idx_t i) const
{
    return mElems[i];
}

template <typename T>
T& DenseVector<T>::operator() (idx_t i)
{
    return mElems[i];
}

template <typename T>
size_t DenseVector<T>::length() const
{
    return mLength;
}

template <typename T>
bool DenseVector<T>::isValid() const
{
    return (mElems != nullptr) && (mLength != 0);
}

template <typename T>
void DenseVector<T>::print(const string& title) const
{
    if( title != "" )
        cout << title << " = " << endl;

    for(size_t i=0;i<mLength;i++)
        cout << setprecision(4) << mElems[i] << '\t';

    cout << endl;
}

template <typename T>
void DenseVector<T>::save(const string& filename, size_t lineSize) const
{
    std::ofstream file;
    file.open(filename.c_str(), ios::out);

    for(size_t i=0;i<mLength;i++)
    {
        file << fixed << setw(6) << setprecision(4) << mElems[i] << '\t';
        if( i % lineSize == lineSize - 1 )
            file << endl;
    }
    file << endl;

    file.close();
}