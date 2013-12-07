#include "StreamViewer.h"


StreamViewer::StreamViewer(QWidget* parent):
	GL2DCanvas(parent)
{
	this->resize(640, 480);
}


StreamViewer::~StreamViewer(void)
{
}

void StreamViewer::initializeGL() {
	makeCurrent();
	GL2DCanvas::initializeGL();

	glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);
	doneCurrent();
}

void StreamViewer::paintGL() {
	GL2DCanvas::paintGL();
	// OpenGL setup
    glClearColor(0,0,0,0);
    glClearDepth(1.0f);
    glEnable(GL_TEXTURE_2D);

    // Camera setup
    glViewport(0, 0, width(), height());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width(), height(), 0, 1, -1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	glBindTexture(GL_TEXTURE_2D, textureId);

	glColor4f(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, 0, 1);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(width(), 0, 1);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(width(), height(), 1);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0, height(), 1);
	glEnd();

	int npts = landmarks.size()/2;
	glColor4f(0, 1, 0, 1);
	glPointSize(3.0);
	glBegin(GL_POINTS);
	for(int i=0;i<npts;i++) {
		glVertex3f(landmarks[i], landmarks[i+npts], 0.0f);
	}
	glEnd();
}

void StreamViewer::resizeGL(int w, int h) {
	GL2DCanvas::resizeGL(w, h);
}

void StreamViewer::bindStreamData(const unsigned char* data, int w, int h) {
	makeCurrent();
	glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h,
                 0, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid*) data);
	update();
	doneCurrent();
}