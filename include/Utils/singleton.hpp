#pragma once

template <typename T>
class Singleton
{
public:
    static T& instance()
    {
        if( mInstance == nullptr )
            mInstance = new T;

        return *mInstance;
    }

    static void destroyInstance()
    {
        delete mInstance;
    }

protected:
  Singleton(){}
  ~Singleton(){}

protected:
  Singleton(Singleton const&){}
  Singleton& operator=(Singleton const&){}


protected:
    static T* mInstance;
};

template <typename T>
T* Singleton<T>::mInstance = nullptr;
