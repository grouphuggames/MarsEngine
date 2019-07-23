#pragma once
#include "Core.h"
#include <gl/GL.h>
#include "glext.h"
#include "wglext.h"

#ifdef _WIN32
#define ASSERT(x) if (!(x)) __debugbreak();
#else
#define ASSERT(x)
#endif
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall())

static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall()
{
	while (u32 error = glGetError())
	{
		MARS_CORE_ERROR(error);
		return false;
	}

	return true;
}


namespace Mars
{
	PFNGLGENBUFFERSPROC glGenBuffers;
	PFNGLBINDBUFFERPROC glBindBuffer;
	PFNGLBUFFERDATAPROC glBufferData;
	PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
	PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
	PFNGLCREATESHADERPROC glCreateShader;
	PFNGLSHADERSOURCEPROC glShaderSource;
	PFNGLCOMPILESHADERPROC glCompileShader;
	PFNGLGETSHADERIVPROC glGetShaderiv;
	PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
	PFNGLCREATEPROGRAMPROC glCreateProgram;
	PFNGLATTACHSHADERPROC glAttachShader;
	PFNGLGETPROGRAMIVPROC glGetProgramiv;
	PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
	PFNGLDELETESHADERPROC glDeleteShader;
	PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
	PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
	PFNGLUSEPROGRAMPROC glUseProgram;
	PFNGLLINKPROGRAMPROC glLinkProgram;
	PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
	PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
	PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
	PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
	PFNGLUNIFORM3FPROC glUniform3f;
	PFNGLUNIFORM1FPROC glUniform1f;
	PFNGLUNIFORM1IPROC glUniform1i;

	void InitGLFunctions()
	{
		glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
		glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
		glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
		glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
		glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
		glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
		glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
		glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
		glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
		glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
		glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
		glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
		glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
		glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");
		glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
		glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");
		glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
		glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
		glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
		wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
		wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
		glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)wglGetProcAddress("glGenerateMipmap");
		glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
		glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
		glUniform3f = (PFNGLUNIFORM3FPROC)wglGetProcAddress("glUniform3f");
		glUniform1f = (PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f");
		glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
	}
}
