#pragma once

#include "../Geometry/matrix.hpp"
#include "../Geometry/point.hpp"
#include "../Math/mathutils.hpp"

namespace PhGUtils {
	inline double rawDepthToMeters( double d ) { 
		return d / 1000.0f;
	}

	inline void depthToWorld(float x, float y, float z, float &X, float &Y, float &Z) {
		const double fx_d = 1.0 / 5.9421434211923247e+02;
		const double fy_d = 1.0 / 5.9104053696870778e+02;
		const double cx_d = 3.3930780975300314e+02;
		const double cy_d = 2.4273913761751615e+02;

		double depth = rawDepthToMeters(z);

		X = ((x - cx_d) * depth * fx_d);
		Y = ((y - cy_d) * depth * fy_d);
		Z = (depth);
	}

	inline int decodeDepth(unsigned char r, unsigned char g, unsigned char b) {
		return (r + ((int)g)<<8 + ((int)b)<<16);
	}

	inline void colorToWorld(float u, float v, float d, float &X, float &Y, float &Z) {
		// focal length
		const double fx_rgb = 5.2921508098293293e+02;
		const double fy_rgb = 5.2556393630057437e+02;

		// original setting
		const float cx_rgb = 3.2894272028759258e+02;
		const float cy_rgb = 2.6748068171871557e+02;

		// for my kinect
		//const float cx_rgb = 3.2894272028759258e+02;
		//const float cy_rgb = 2.5848068171871557e+02;

		// for the Yen-Lin's data
		//const double cx_rgb = 3.3394272028759258e+02;
		//const double cy_rgb = 2.4048068171871557e+02;

		// projection mapping
		//u = clamp((int)((x * fx_rgb / z) + cx_rgb), 0, 639);
		//v = clamp((int)((y * fy_rgb / z) + cy_rgb), 0, 479);

		double depth = rawDepthToMeters(d);

		// inverse mapping of projection
		X = (u - cx_rgb) * depth / fx_rgb;
		Y = (v - cy_rgb) * depth / fy_rgb;
		Z = -depth;
	}

	inline void worldToColor(float x, float y, float z, int& u, int& v) {
		const Matrix4x4d rotMat(
			9.9984628826577793e-01, 1.2635359098409581e-03, -1.7487233004436643e-02, 0,
			-1.4779096108364480e-03, 9.9992385683542895e-01, -1.2251380107679535e-02, 0,
			1.7470421412464927e-02, 1.2275341476520762e-02, 9.9977202419716948e-01, 0,
			0, 0, 0, 1
			);

		const Matrix4x4d transMat(
			1, 0, 0, 1.9985242312092553e-02,
			0, 1, 0, -7.4423738761617583e-04,
			0, 0, 1, -1.0916736334336222e-02,
			0, 0, 0, 1
			);

		const Matrix4x4d finalMat = rotMat.transposed() * (transMat) * -1;

		const double fx_rgb = 5.2921508098293293e+02;
		const double fy_rgb = 5.2556393630057437e+02;

		// original setting
		//const float cx_rgb = 3.2894272028759258e+02;
		//const float cy_rgb = 2.6748068171871557e+02;

		// for my kinect
		const float cx_rgb = 3.2894272028759258e+02;
		const float cy_rgb = 2.5848068171871557e+02;

		// for the Yen-Lin's data
		//const double cx_rgb = 3.3394272028759258e+02;
		//const double cy_rgb = 2.4048068171871557e+02;

		Point4d transformedPos = finalMat * Point4d(x, y, z, 1);
		Point4d pos;
		if(transformedPos.w == 0 || transformedPos.w == 1)
			pos = transformedPos;
		else
			pos = transformedPos / transformedPos.w;	

		double invZ = 1.0 / pos.z;

		u = clamp((int)((pos.x * fx_rgb * invZ) + cx_rgb), 0, 639);
		v = clamp((int)((pos.y * fy_rgb * invZ) + cy_rgb), 0, 479);
	}

}