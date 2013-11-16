#include "Timer.h"


Timer::Timer(void)
{
}


Timer::~Timer(void)
{
}

void Timer::tic() {
	QueryPerformanceCounter((LARGE_INTEGER *) &start);
}

void Timer::toc() {
	QueryPerformanceCounter((LARGE_INTEGER *) &end);
}