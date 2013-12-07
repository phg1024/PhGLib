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
	enum Mode{
		Separate = 0,	// separate color and depth images
		WarpDepth,		// warp depth to match color
		WarpColor,		// warp color to match depth
		ModeCount
	};

	KinectManager(KinectManager::Mode m = Separate);
	~KinectManager();

	bool initKinect();
	void switchMode();

	const int getHeight() const { return height; }
	const int getWidth() const { return width; }

	void updateStream();
	const vector<unsigned char>& getRGBData() const{ return rgbdata; }
	const vector<unsigned char>& getDepthData() const{ return depthdata; }
	const vector<USHORT>& getDepthValues() const { return depthValues; }

	KinectManager::Mode getMode() const { return m; }
	void setMode(KinectManager::Mode val) { m = val; }

protected:
	bool retrieveRGBData();
	bool retrieveDepthData();

	void directMapColor();
	void directMapDepth();
	void warpDepthToColor();
	void warpColorToDepth();

private:
	Mode m;

	// Kinect variables
	HANDLE rgbStream;              // The identifier of the Kinect's RGB Camera
	HANDLE depthStream;
	INuiSensor* sensor;            // The kinect sensor

	static const int width = 640;
	static const int height = 480;

	vector<long> colorCoordinates;	// the color coordinates for each depth pixel
	vector<USHORT> depthValues;
	vector<unsigned char> colorValues;
	vector<unsigned char> rgbdata;
	vector<unsigned char> depthdata;
};

}