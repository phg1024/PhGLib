#include "kinectmanager.h"

namespace PhGUtils {
KinectManager::KinectManager()
{
	initKinect();
}

KinectManager::~KinectManager()
{
	sensor->NuiShutdown();
}

bool KinectManager::initKinect() {
	// Get a working kinect sensor
	int numSensors;
	if (NuiGetSensorCount(&numSensors) < 0 || numSensors < 1) return false;
	if (NuiCreateSensorByIndex(0, &sensor) < 0) return false;

	// Initialize sensor
	sensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_COLOR | NUI_INITIALIZE_FLAG_USES_DEPTH);

	sensor->NuiImageStreamOpen(
		NUI_IMAGE_TYPE_COLOR,            // Depth camera or rgb camera?
		NUI_IMAGE_RESOLUTION_640x480,    // Image resolution
		0,      // Image stream flags, e.g. near mode
		2,      // Number of frames to buffer
		NULL,   // Event handle
		&rgbStream);

	sensor->NuiImageStreamOpen(
		NUI_IMAGE_TYPE_DEPTH,            // Depth camera or rgb camera?
		NUI_IMAGE_RESOLUTION_640x480,    // Image resolution
		0,      // Image stream flags, e.g. near mode
		2,      // Number of frames to buffer
		NULL,   // Event handle
		&depthStream);

	rgbdata.resize(width * height * 4);
	depthdata.resize(width * height * 4);

	m = Separate;
	
	return sensor;
}

bool KinectManager::updateRGBData() {
	NUI_IMAGE_FRAME imageFrame;
	NUI_LOCKED_RECT LockedRect;
	if (sensor->NuiImageStreamGetNextFrame(rgbStream, 0, &imageFrame) < 0) return false;
	INuiFrameTexture* texture = imageFrame.pFrameTexture;
	texture->LockRect(0, &LockedRect, NULL, 0);

	if (LockedRect.Pitch != 0)
	{
		const BYTE* curr = (const BYTE*) LockedRect.pBits;
		const BYTE* dataEnd = curr + (width*height)*4;

		unsigned char* dest = &(rgbdata[0]);

		while (curr < dataEnd) {
			*dest++ = *curr++;
			*dest++ = *curr++;
			*dest++ = *curr++;
			*dest++ = 0xff;
			curr++;
		}
	}

	texture->UnlockRect(0);
	sensor->NuiImageStreamReleaseFrame(rgbStream, &imageFrame);
	return true;
}

bool KinectManager::updateDepthData() {
	NUI_IMAGE_FRAME imageFrame;
	NUI_LOCKED_RECT LockedRect;

	if (sensor->NuiImageStreamGetNextFrame(depthStream, 0, &imageFrame) < 0) return false;
	INuiFrameTexture* texture = imageFrame.pFrameTexture;
	texture->LockRect(0, &LockedRect, NULL, 0);

	if (LockedRect.Pitch != 0)
	{
		const USHORT* curr = (const USHORT*) LockedRect.pBits;
		const USHORT* dataEnd = curr + (width*height);

		unsigned char* dest = &(depthdata[0]);

		for(int i=0, didx=0, dstidx=0;i<height;i++) {
			for(int j=0;j<width;j++, didx++, dstidx+=4) {
				USHORT depth = NuiDepthPixelToDepth(curr[didx]);

				// clamp at 2047

				if( m == ColorOver ) {
					long x, y;

					NuiImageGetColorPixelCoordinatesFromDepthPixelAtResolution(
						NUI_IMAGE_RESOLUTION_640x480, NUI_IMAGE_RESOLUTION_640x480, NULL,
						j, i, depth<<3, &x, &y);

					unsigned char r, g, b;
					if (x < 0 || y < 0 || x > width || y > height || depth == 0) {
						r = g = b = 0;
					}
					else {
						const BYTE* rgb = &(rgbdata[0]) + (x + width*y)*4;
						r = rgb[2]; g = rgb[1]; b = rgb[0];
					}

					dest[dstidx] = b;
					dest[dstidx+1] = g;
					dest[dstidx+2] = r;
					dest[dstidx+3] = 0xff;
				}
				else{
					dest[dstidx] = 0;
					dest[dstidx+1] = (BYTE) (depth/256);
					dest[dstidx+2] = (BYTE) (depth%256);
					dest[dstidx+3] = 0xff;
				}
			}
		}
	}

	texture->UnlockRect(0);
	sensor->NuiImageStreamReleaseFrame(depthStream, &imageFrame);
	return true;
}

void KinectManager::switchMode() {
	m = Mode((m+1)%2);
}

void KinectManager::updateStream() {
	if( !updateRGBData() ) {
		throw "Unable to get RGB data!";
	}

	if( !updateDepthData() ) {
		throw "Unable to get RGB data!";
	}
}

}