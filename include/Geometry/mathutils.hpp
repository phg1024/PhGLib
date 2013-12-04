#pragma once

namespace PhGUtils {
/* value of Pi */
static const double PI = 3.14159265;

template <typename T>
/* 1D Gaussian function */
T gaussianFunc(T mu, T sigma, T x)
{
	T diff = x - mu;
	T sigma2 = sigma * sigma;
	return 1.0 / sqrt(2.0 * PI * sigma2) * exp( -(diff*diff)/(2.0 * sigma2) );
}

template <typename T>
T clamp(T val, T lower, T upper) {
	if( val < lower ) return lower;
	if( val > upper ) return upper;
	return val;
}

}