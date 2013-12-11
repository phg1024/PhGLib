#pragma once

#include <Windows.h>

#include "../phgutils.h"

namespace PhGUtils {

class Timer
{
public:
	Timer(void);
	~Timer(void);

	void reset();
	void tic();
	void toc();
	void toc(const string& msg);

	float elapsed() {
		return totalElapse;
	}
	float gap() {
        __int64 freq = 0;
		QueryPerformanceFrequency((LARGE_INTEGER *) &freq);
		return ((end - start) * 1.0 / freq);
	}
private:
	__int64 start, end;
	float totalElapse;
};

}