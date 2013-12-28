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
		0, 0, -1.0, 0,
		0, 0, -1.0, 0
		);

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
		X = (u - cx_rgb) * depth / fx_rgb;
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
		u = clamp<float>(cx_rgb + x * fx_rgb * invZ, 0, 639.f);
		v = clamp<float>(cy_rgb + y * fy_rgb * invZ, 0, 479.f);
		d = -metersToRawDepth(z);
	}

}