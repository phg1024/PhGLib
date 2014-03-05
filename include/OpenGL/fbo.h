#pragma once

#include "../phgutils.h"

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "glutilities.h"

namespace PhGUtils {

/*	
A simple framebuffer object class
Peihong created this class following the example at http://www.songho.ca/opengl/gl_fbo.html
*/
class FBO {
public:
	FBO(int width, int height):width(width), height(height) {
		init();
	}
	~FBO();

	void bind();
	void release();

	bool checkStatus();
	void printInfo();

	bool isValid() const { return valid; }
	int getWidth() const { return width; }
	int getHeight() const { return height; }

	GLuint getID() const { return id; }
	GLuint getColorTextureID() const { return color_buffer; }
	GLuint getDepthTextureID() const { return depth_buffer; }

protected:
	void init();

private:
	int width, height;

	bool valid;

	GLuint id;
	GLuint color_buffer;
	GLuint depth_buffer;
};

}