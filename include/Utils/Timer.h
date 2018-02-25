#pragma once

#ifdef WIN32
#include <Windows.h>
#else
#include <chrono>
using namespace std::chrono;
#endif

#include "../phgutils.h"

namespace PhGUtils {

#ifdef WIN32
class Timer
{
public:
	Timer(void);
	~Timer(void);

	void reset();
	void tic();
	void toc();
	void toc(const string& msg);
    void tocMS(const string& msg);

	float elapsed() const {
		return totalElapse;
	}
	float gap() const {
        __int64 freq = 0;
		QueryPerformanceFrequency((LARGE_INTEGER *) &freq);
		return ((end - start) * 1.0 / freq);
	}
private:
	__int64 start, end;
	float totalElapse;
};
#else
class Timer {
public:
    Timer(void);
    ~Timer(void);

    void reset();
    void tic();
    void toc();
    void toc(const string& msg);
    void report(const string& msg);

    float elapsed() const;
    float gap() const;

private:
    high_resolution_clock::time_point start, end;
    float totalElapse;
};

#endif
}
