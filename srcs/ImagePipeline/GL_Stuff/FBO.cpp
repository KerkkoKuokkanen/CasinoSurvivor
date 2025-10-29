
#include "FBO.h"
#include <iostream>
#include "pillarBoxes.h"

void FBO::InitFBO()
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &colorTexture);
	glBindTexture(GL_TEXTURE_2D, colorTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "Error: Framebuffer is not complete!" << std::endl;

	// Check for OpenGL errors
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
		std::cerr << "OpenGL error during FBO initialization: " << error << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FBO::FBO(int width, int height)
{
	fbo = 0;
	FBO::width = width;
	FBO::height = height;
	colorTexture = 0;
	InitFBO();
}

FBO::~FBO()
{
	if (fbo)
		glDeleteFramebuffers(1, &fbo);
	if (colorTexture) 
		glDeleteTextures(1, &colorTexture);
}

void FBO::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glViewport(0, 0, width, height);
}

void BindScreenForUse()
{
	SetViewPort();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
