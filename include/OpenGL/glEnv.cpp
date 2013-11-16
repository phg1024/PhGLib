#include "glEnv.h"
#include <cstdio>

void checkGLError(const char *situation)
{
	GLuint err = glGetError();
	if (err != GL_NO_ERROR) {
		const GLubyte *errString = gluErrorString(err);
		printf("%s [ERROR]: %s\n", situation, errString);
	}
}
