#pragma once

#define NOMINMAX
#include <Windows.h>
#include <NuiApi.h>
#include <NuiImageCamera.h>
#include <NuiSensor.h>

#include "../phgutils.h"

namespace PhGUtils {

class KinectManager {
public:
	KinectManager();
	~KinectManager();

	bool initKinect();
	void switchMode();

	const int getHeight() const { return height; }
	const int getWidth() const { return width; }

	void updateStream();
	const vector<unsigned char>& getRGBData() const{ return rgbdata; }
	const vector<unsigned char>& getDepthData() const{ return depthdata; }

protected:
	bool updateRGBData();
	bool updateDepthData();

private:
	enum Mode{
		Separate = 0,
		ColorOver
	} m;

	// Kinect variables
	HANDLE rgbStream;              // The identifier of the Kinect's RGB Camera
	HANDLE depthStream;
	INuiSensor* sensor;            // The kinect sensor

	static const int width = 640;
	static const int height = 480;

	vector<unsigned char> rgbdata;
	vector<unsigned char> depthdata;
};

}