#include "common.h"
#include "singleton.hpp"

class MemoryCounter;

template <typename T>
class ArrayAllocator : public Singleton<ArrayAllocator<T> >
{
public:
    T* allocate(size_t size)
    {
        T* ptr = new T[size];
        if( ptr != nullptr )
        {
            size_t allocate_size = size * sizeof(T);

            MemoryCounter::instance().add( allocate_size );

            //cout << "allocated " << allocate_size << " bytes" << endl;
            //cout << "currently allocated " << MemoryCounter::instance().size_byte() << " bytes" << endl;

            return ptr;
        }
        else
            return nullptr;
    }

    void release(T* ptr, size_t size)
    {
        if( ptr != nullptr )
        {
            size_t release_size = size * sizeof(T);

            //cout << "released " << release_size << " bytes" << endl;
            delete[] ptr;
            MemoryCounter::instance().sub( release_size );
            //cout << "currently allocated " << MemoryCounter::instance().size_byte() << " bytes" << endl;
        }
    }
};

class MemoryCounter : public Singleton<MemoryCounter>
{
public:
    MemoryCounter():mPeakUsage(0), mTotalBytes(0){}
    ~MemoryCounter(){}

    void add(size_t size)
    {
        mTotalBytes += size;
        if( mTotalBytes > mPeakUsage )
            mPeakUsage = mTotalBytes;
    }

    void sub(size_t size){
        mTotalBytes -= size;
    }

    double size_byte(){ return mTotalBytes; }
    double size_kb(){ return mTotalBytes / 1024.0; }
    double size_mb(){ return mTotalBytes / 1048576.0; }

    double size_mb_peak(){ return mPeakUsage / 1048576.0; }
    double size_byte_peak(){ return mPeakUsage; }
    double size_kb_peak(){ return mPeakUsage / 1024.0; }

private:
    size_t mPeakUsage;
    size_t mTotalBytes;
};
