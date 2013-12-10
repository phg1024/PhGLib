#include "Timer.h"
#include "utility.hpp"
#include "stringutils.h"

namespace PhGUtils {
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

void Timer::toc(const string& msg) {
	QueryPerformanceCounter((LARGE_INTEGER *) &end);
	message("Time cost for " + msg + " = " + toString(gap()) + " seconds");
}

}