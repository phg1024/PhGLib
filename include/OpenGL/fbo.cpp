#include "fbo.h"

namespace PhGUtils {

void FBO::init()
{
	// color texture
	glGenTextures(1, &color_buffer);
	glBindTexture(GL_TEXTURE_2D, color_buffer);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap generation included in OpenGL v1.4
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// create a framebuffer object, you need to delete them when program exits.
	glGenFramebuffers(1, &id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);

	// create a renderbuffer object to store depth info
	// NOTE: A depth renderable image should be attached the FBO for depth test.
	// If we don't attach a depth renderable image to the FBO, then
	// the rendering output will be corrupted because of missing depth test.
	// If you also need stencil test for your rendering, then you must
	// attach additional image to the stencil attachement point, too.
	glGenRenderbuffers(1, &depth_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// attach a texture to FBO color attachement point
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_buffer, 0);

	// attach a render buffer to depth attachment point
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer);

	// check FBO status
	printInfo();
	valid = checkStatus();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::printInfo()
{
	std::cout << "\n===== FBO STATUS =====\n";

	// print max # of colorbuffers supported by FBO
	int colorBufferCount = 0;
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &colorBufferCount);
	std::cout << "Max Number of Color Buffer Attachment Points: " << colorBufferCount << std::endl;

	int objectType;
	int objectId;

	// print info of the colorbuffer attachable image
	for(int i = 0; i < colorBufferCount; ++i)
	{
		glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0+i,
			GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
			&objectType);
		if(objectType != GL_NONE)
		{
			glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
				GL_COLOR_ATTACHMENT0+i,
				GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
				&objectId);

			std::string formatName;

			std::cout << "Color Attachment " << i << ": ";
			if(objectType == GL_TEXTURE)
			{
				std::cout << "GL_TEXTURE, " << getTextureParameters(objectId) << std::endl;
			}
			else if(objectType == GL_RENDERBUFFER)
			{
				std::cout << "GL_RENDERBUFFER, " << getRenderbufferParameters(objectId) << std::endl;
			}
		}
	}

	// print info of the depthbuffer attachable image
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
		GL_DEPTH_ATTACHMENT,
		GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
		&objectType);
	if(objectType != GL_NONE)
	{
		glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
			GL_DEPTH_ATTACHMENT,
			GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
			&objectId);

		std::cout << "Depth Attachment: ";
		switch(objectType)
		{
		case GL_TEXTURE:
			std::cout << "GL_TEXTURE, " << getTextureParameters(objectId) << std::endl;
			break;
		case GL_RENDERBUFFER:
			std::cout << "GL_RENDERBUFFER, " << getRenderbufferParameters(objectId) << std::endl;
			break;
		}
	}

	// print info of the stencilbuffer attachable image
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
		GL_STENCIL_ATTACHMENT,
		GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
		&objectType);
	if(objectType != GL_NONE)
	{
		glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
			GL_STENCIL_ATTACHMENT,
			GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
			&objectId);

		std::cout << "Stencil Attachment: ";
		switch(objectType)
		{
		case GL_TEXTURE:
			std::cout << "GL_TEXTURE, " << getTextureParameters(objectId) << std::endl;
			break;
		case GL_RENDERBUFFER:
			std::cout << "GL_RENDERBUFFER, " << getRenderbufferParameters(objectId) << std::endl;
			break;
		}
	}

	std::cout << std::endl;
}

bool FBO::checkStatus()
{
    // check FBO status
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch(status)
    {
    case GL_FRAMEBUFFER_COMPLETE:
        std::cout << "Framebuffer complete." << std::endl;
        return true;

    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        std::cout << "[ERROR] Framebuffer incomplete: Attachment is NOT complete." << std::endl;
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        std::cout << "[ERROR] Framebuffer incomplete: No image is attached to FBO." << std::endl;
        return false;
/*
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
        std::cout << "[ERROR] Framebuffer incomplete: Attached images have different dimensions." << std::endl;
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS:
        std::cout << "[ERROR] Framebuffer incomplete: Color attached images have different internal formats." << std::endl;
        return false;
*/
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        std::cout << "[ERROR] Framebuffer incomplete: Draw buffer." << std::endl;
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
        std::cout << "[ERROR] Framebuffer incomplete: Read buffer." << std::endl;
        return false;

    case GL_FRAMEBUFFER_UNSUPPORTED:
        std::cout << "[ERROR] Framebuffer incomplete: Unsupported by FBO implementation." << std::endl;
        return false;

    default:
        std::cout << "[ERROR] Framebuffer incomplete: Unknown error." << std::endl;
        return false;
    }
}

void FBO::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void FBO::release()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FBO::~FBO()
{
	glDeleteTextures(1, &color_buffer);
	glDeleteFramebuffers(1, &id);
	glDeleteRenderbuffers(1, &depth_buffer);
}

}