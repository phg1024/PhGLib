#include "Timer.h"
#include "utility.hpp"
#include "stringutils.h"

namespace PhGUtils {

#ifdef WIN32
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
	message(msg + " time: " + toString(gap()) + " seconds.");
	totalElapse += gap();
}

void Timer::tocMS(const string& msg) {
	QueryPerformanceCounter((LARGE_INTEGER *) &end);
	message(msg + " time = " + toString(gap()*1000.0) + " ms.");
	totalElapse += gap();
}

void Timer::reset()
{
	totalElapse = 0;
}
#else
Timer::Timer(void):totalElapse(0){}

Timer::~Timer(void){}

void Timer::tic() {
    start = high_resolution_clock::now();
}

void Timer::toc() {
    end = high_resolution_clock::now();
    totalElapse += gap();
}

void Timer::toc(const string& msg) {
    end = high_resolution_clock::now();
    message("Time cost for " + msg + " = " + toString(gap()) + " seconds");
    totalElapse += gap();
}

void Timer::reset() {
    totalElapse = 0;
}

#endif
}
