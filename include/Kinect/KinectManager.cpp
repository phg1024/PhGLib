#include "kinectmanager.h"
#include "../Utils/utility.hpp"

namespace PhGUtils {
	KinectManager::KinectManager(KinectManager::Mode m):
		m(m)
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
		colorValues.resize(width * height * 4);
		depthdata.resize(width * height * 4);
		depthValues.resize(width * height);

		colorCoordinates.resize(width * height * 2);

		m = Separate;

		return sensor;
	}

	bool KinectManager::retrieveRGBData() {
		NUI_IMAGE_FRAME imageFrame;
		NUI_LOCKED_RECT LockedRect;
		if (sensor->NuiImageStreamGetNextFrame(rgbStream, 0, &imageFrame) < 0) return false;
		INuiFrameTexture* texture = imageFrame.pFrameTexture;
		texture->LockRect(0, &LockedRect, NULL, 0);

		if (LockedRect.Pitch != 0)
		{
			memcpy(&(colorValues[0]), LockedRect.pBits, LockedRect.size);
		}

		texture->UnlockRect(0);
		sensor->NuiImageStreamReleaseFrame(rgbStream, &imageFrame);
		return true;
	}

	bool KinectManager::retrieveDepthData() {
		NUI_IMAGE_FRAME imageFrame;
		NUI_LOCKED_RECT LockedRect;

		if (sensor->NuiImageStreamGetNextFrame(depthStream, 0, &imageFrame) < 0) return false;
		INuiFrameTexture* texture = imageFrame.pFrameTexture;
		texture->LockRect(0, &LockedRect, NULL, 0);

		// get the color coordinates
		if( m != Separate ) {
			sensor->NuiImageGetColorPixelCoordinateFrameFromDepthPixelFrameAtResolution(
				NUI_IMAGE_RESOLUTION_640x480,
				NUI_IMAGE_RESOLUTION_640x480,
				width * height,
				reinterpret_cast<USHORT*>(LockedRect.pBits),
				width * height * 2,
				&(colorCoordinates[0])
				);
		}

		// store the raw depth value for later use
		if( LockedRect.Pitch != 0 ) {
			memcpy(&(depthValues[0]), LockedRect.pBits, LockedRect.size);
		}		

		texture->UnlockRect(0);
		sensor->NuiImageStreamReleaseFrame(depthStream, &imageFrame);
		return true;
	}

	void KinectManager::switchMode() {
		m = Mode((m+1)%ModeCount);

		switch( m ) {
		case Separate: {
			message("Kinect: separate mode.");
			break;
					   }
		case WarpDepth: {
			message("Kinect: warp depth.");
			break;
						}
		case WarpColor: {
			message("Kinect: warp color.");
			break;
						}
		}
	}

	void KinectManager::updateStream() {
		if( !retrieveRGBData() ) {
			throw "Unable to get RGB data!";
		}

		if( !retrieveDepthData() ) {
			throw "Unable to get RGB data!";
		}

		// post process
		switch ( m )
		{
		default:
		case Separate: {
			// separate mode, just copy the depth values to the depth data
			directMapColor();
			directMapDepth();
			return;
					   }
		case WarpDepth: {
			directMapColor();
			warpDepthToColor();
			return;
						}
		case WarpColor: {
			directMapDepth();
			warpColorToDepth();
			return;
						}
		}
	}

	void KinectManager::directMapColor() {
		const BYTE* curr = (const BYTE*) &(colorValues[0]);
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

	void KinectManager::directMapDepth() {
		const USHORT* curr = (const USHORT*) &(depthValues[0]);
		const USHORT* dataEnd = curr + (width*height);

		unsigned char* dest = &(depthdata[0]);

		for(int i=0, didx=0,dstidx=0;i<height;i++) {
			for(int j=0;j<width;j++, didx++,dstidx+=4) {
				USHORT depth = NuiDepthPixelToDepth(curr[didx]);
				dest[dstidx] = 0;
				dest[dstidx+1] = (BYTE) (depth/256);
				dest[dstidx+2] = (BYTE) (depth%256);
				dest[dstidx+3] = 0xff;
			}
		}
	}

	void KinectManager::warpDepthToColor() {
		// warp the depth image to match with color
		for(int i=0, didx=0;i<height;++i) {
			for(int j=0;j<width;++j, ++didx) {
				int cidx = didx<<1;
				long x = colorCoordinates[cidx];
				long y = colorCoordinates[cidx+1];

				if( x >= 0 && y >= 0 && x < width && y < height ) {
					// destination
					int pidx = (y*width + x) * 4;
					USHORT depth = NuiDepthPixelToDepth(depthValues[didx]);

					depthdata[pidx] = 0;
					depthdata[++pidx] = (BYTE) (depth/256);
					depthdata[++pidx] = (BYTE) (depth%256);
					depthdata[++pidx] = 0xff;
				}
			}
		}

		// apply median blur to the depth data for smoother results
	}

	void KinectManager::warpColorToDepth() {
		// warp the color image to match with depth
		for(int i=0, didx=0;i<height;++i) {
			for(int j=0;j<width;++j, ++didx) {
				int cidx = didx<<1;
				long x = colorCoordinates[cidx];
				long y = colorCoordinates[cidx+1];

				if( x >= 0 && y >= 0 && x < width && y < height ) {
					// source rgb value index
					int sidx = (y * width + x) * 4;

					// destination
					int pidx = didx << 2;

					rgbdata[pidx] = colorValues[sidx];
					rgbdata[++pidx] = colorValues[++sidx];
					rgbdata[++pidx] = colorValues[++sidx];
					rgbdata[++pidx] = 0xff;
				}
			}
		}
	}

}