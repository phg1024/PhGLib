#pragma once

#include "../Geometry/matrix.hpp"
#include "../Geometry/point.hpp"
#include "../Math/mathutils.hpp"

namespace PhGUtils {
	__forceinline float rawDepthToMeters( float d ) { 
		return d / 1000.0f;
	}

	__forceinline float metersToRawDepth( float v ) {
		return v * 1000.0f;
	}

	/*
	__forceinline void depthToWorld(float x, float y, float z, float &X, float &Y, float &Z) {
		const double fx_d = 1.0 / 5.9421434211923247e+02;
		const double fy_d = 1.0 / 5.9104053696870778e+02;
		const double cx_d = 3.3930780975300314e+02;
		const double cy_d = 2.4273913761751615e+02;

		double depth = rawDepthToMeters(z);

		X = ((x - cx_d) * depth * fx_d);
		Y = ((y - cy_d) * depth * fy_d);
		Z = (depth);
	}
	*/

	__forceinline int decodeDepth(unsigned char r, unsigned char g, unsigned char b) {
		return (r + ((int)g)<<8 + ((int)b)<<16);
	}

	static const Matrix4x4f KinectColorProjection(
		525.0/320.0, 0, 0, 0,
		0, 525.0/240.0, 0, 0,
		0, 0, -10.001/9.999, -0.02/9.999,	// near 0.001, far 10.0
		0, 0, -1.0, 0
		);

  __forceinline void colorToWorld(float u, float v, float d, float &x, float &y, float &z,
    float fx, float cx, float cy) {

    const float farval = 1000.0, nearval = 0.00001;
    const float e33 = -(farval + nearval) / (farval - nearval), e34 = -2.0*farval*nearval / (farval - nearval);
    // d = (e33 * Z + e34) / (-Z)
    // d = -e34/Z - e33;

    // M_proj * (x; y; z)
    // X = e11 * x 
    // Y = e22 * y
    // Z = e33 * z + e34
    // W = -z;

    // normalize it
    // xc = X/W = -(e11 * x / z + e13)
    // yc = Y/W = -(e22 * y / z + e23)
    // zc = Z/W = -(e33 + e34 / z)

    // viewport transform
    // u = xc * cx + cx
    // v = yc * cy + cy
    // d = (farval - nearval)/2 * zc + (farval + nearval)/2

    // compute normalized device coordinates
    // zc = (d - (farval + nearval)/2) / (farval - nearval)/2
    float zc = (d - (farval + nearval)*0.5) / ((farval - nearval)*0.5);
    float yc = (v - cy) / cy;
    float xc = (u - cx) / cx;

    // compute world coordinates
    z = -e34 / (zc + e33);
    y = -yc * z / fx;
    x = -xc * z / fx;
  }

  // inverse function of colorToWorld
  __forceinline void worldToColor(float x, float y, float z, float& u, float& v, float& d,
      float fx, float cx, float cy) {

    const float farval = 1000.0, nearval = 0.00001;
    const float e33 = -(farval + nearval) / (farval - nearval), e34 = -2.0*farval*nearval / (farval - nearval);

    // M_proj * (x; y; z)
    // X = e11 * x 
    // Y = e22 * y
    // Z = e33 * z + e34
    // W = -z;
    float X = fx * x / cx;
    float Y = fx * y / cy;
    float Z = e33 * z + e34;
    float W = -z;

    // normalize it
    // xc = X/W = -(e11 * x / z + e13)
    // yc = Y/W = -(e22 * y / z + e23)
    // zc = Z/W = -(e33 + e34 / z)
    float xc = X / W, yc = Y / W, zc = Z / W;

    // viewport transform
    // u = xc * cx + cx
    // v = yc * cy + cy
    // d = (farval - nearval)/2 * zc + (farval + nearval)/2
    
    u = xc * cx + cx;
    v = yc * cy + cy;
    d = (farval - nearval)*0.5 * zc + (farval + nearval)*0.5;
  }

	__forceinline void colorToWorld(float u, float v, float d, float &X, float &Y, float &Z) {
		// focal length
		const float fx_rgb = 525.0, fy_rgb = 525.0;
		// for 640x480 image
		const float cx_rgb = 320.0, cy_rgb = 240.0;

		// This part is correct now.
		// Given a Kinect depth value, its depth in OpenGL coordinates
		// system must be negative.
		float depth = -rawDepthToMeters(d);

		// inverse mapping of projection
		X = -(u - cx_rgb) * depth / fx_rgb;
		Y = (v - cy_rgb) * depth / fy_rgb;

		Z = depth;
	}

	// inverse function of colorToWorld
	__forceinline void worldToColor(float x, float y, float z, float& u, float& v, float& d) {
		// focal length
		const float fx_rgb = 525.0, fy_rgb = 525.0;
		// for 640x480 image
		const float cx_rgb = 320.0, cy_rgb = 240.0;

		float invZ = 1.0 / z;
		u = clamp<float>(cx_rgb - x * fx_rgb * invZ, 0, 639.f);
		v = clamp<float>(cy_rgb + y * fy_rgb * invZ, 0, 479.f);
		d = -metersToRawDepth(z);
	}

}