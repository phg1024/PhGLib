#include "Timer.h"
#include "utility.hpp"
#include "stringutils.h"

namespace PhGUtils {
Timer::Timer(void):totalElapse(0){}

Timer::~Timer(void){}

void Timer::tic() {
	QueryPerformanceCounter((LARGE_INTEGER *) &start);
}

void Timer::toc() {
	QueryPerformanceCounter((LARGE_INTEGER *) &end);
	totalElapse += gap();
}

void Timer::toc(const string& msg) {
	QueryPerformanceCounter((LARGE_INTEGER *) &end);
	message("Time cost for " + msg + " = " + toString(gap()) + " seconds");
	totalElapse += gap();
}

void Timer::reset()
{
	totalElapse = 0;
}

}