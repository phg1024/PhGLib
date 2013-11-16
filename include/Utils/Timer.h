#pragma once

#include <Windows.h>

class Timer
{
public:
	Timer(void);
	~Timer(void);

	void tic();
	void toc();

	float gap() {
        __int64 freq = 0;

        // Start timing the code.
		QueryPerformanceFrequency((LARGE_INTEGER *) &freq);
                
		return ((end - start) * 1.0 / freq);
	}
private:
	__int64 start, end;
};

