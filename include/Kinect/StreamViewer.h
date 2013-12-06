#pragma once

#include "../OpenGL/gl2dcanvas.h"

class StreamViewer : public GL2DCanvas
{
public:
	StreamViewer(QWidget* parent = 0);
	~StreamViewer(void);

	void bindStreamData(const unsigned char* data, int, int);
	void bindLandmarks(const vector<float>& f) {
		landmarks = f;
		update();
	}
protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int w, int h);

private:
	GLuint textureId;
	vector<float> landmarks;
};

