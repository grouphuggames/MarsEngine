#pragma once
#include "Core.h"
#include "vec3.h"
#include "vec4.h"
#include "mat4.h"
#include "Debug.h"
#include <gl/GL.h>
#include "glext.h"
#include "wglext.h"


namespace Mars
{
	// OpenGL & Vulkan
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

	const char *vertexShaderSource = "#version 460 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aColor;\n"
		"layout (location = 2) in vec2 aTexCoord;\n"
		"out vec3 ourColor;\n"
		"out vec2 TexCoord;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos, 1.0);\n"
		"	ourColor = aColor;\n"
		"	TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
		"}\0";
	const char *fragmentShaderSource = "#version 460 core\n"
		"out vec4 FragColor;\n"
		"in vec3 ourColor;\n"
		"in vec2 TexCoord;\n"
		"uniform sampler2D texture1;\n"
		"void main()\n"
		"{\n"
		"   FragColor = texture(texture1, TexCoord);\n"
		"}\n\0";

	const s32 pixel_format_attrib_list[] = 
	{
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_COLOR_BITS_ARB, 32,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		0
	};

	const s32 context_attrib_list[] = 
	{
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
		0
	};

	void InitGL()
	{
#ifdef _WIN32
		PIXELFORMATDESCRIPTOR pfd = 
		{
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA,
			32,
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			24,
			8,
			0,
			PFD_MAIN_PLANE,
			0,
			0, 0, 0
		};

		HDC device_context = GetDC(game_state.hwnd);
		s32 tmp_pixel_format_index = ChoosePixelFormat(device_context, &pfd);

		if (tmp_pixel_format_index == 0)
		{
			MARS_CORE_ERROR("Failed to get pixel format!");
			return;
		}

		if (!SetPixelFormat(device_context, tmp_pixel_format_index, &pfd))
		{
			MARS_CORE_ERROR("Could not set pixel format!");
			return;
		}

		HGLRC tmp_render_context = wglCreateContext(device_context);
		if (tmp_render_context == NULL)
		{
			MARS_CORE_ERROR("Couldn't create OpenGL render context!");
			return;
		}

		wglMakeCurrent(device_context, tmp_render_context);

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

		s32 pixel_format_index;
		u32 num_formats;

		wglChoosePixelFormatARB(device_context, pixel_format_attrib_list, NULL, 1, &pixel_format_index, &num_formats);

		if (!SetPixelFormat(device_context, pixel_format_index, &pfd))
		{
			MARS_CORE_ERROR("Could not set pixel format!");
			return;
		}

		game_state.render_context = wglCreateContextAttribsARB(device_context, NULL, context_attrib_list);

		wglDeleteContext(tmp_render_context);
		wglMakeCurrent(device_context, game_state.render_context);
		MARS_CORE_INFO("OpenGL Version: ", (char*)glGetString(GL_VERSION));
#endif
	}

	struct GLTexture
	{
		unsigned char* data = nullptr;
		s32 width;
		s32 height;
	};

	GLTexture LoadBMP(const char* path)
	{
		GLTexture res;

		FILE* file = fopen(path, "rb");
		if (!file)
		{
			MARS_CORE_ERROR("Cannot open file: ", path);
			return GLTexture();
		}

		unsigned char header[54];
		if (fread(header, 1, 54, file) != 54 || header[0]!='B' || header[1]!='M')
		{
			MARS_CORE_ERROR("File: ", path, " is not a BMP!");
			return GLTexture();
		}

		u32 data_position = *(int*)&(header[0x0A]);
		u32 image_size = *(int*)&(header[0x22]);
		res.width = *(int*)&(header[0x12]);
		res.height = *(int*)&(header[0x16]);

		if (image_size == 0) { image_size = 3 * res.width * res.height; }
		if (data_position == 0) { data_position = 54; }

		res.data = new unsigned char[image_size];	// as of right now, this memory is not being cleaned up...
		MARS_CORE_WARN("Memory here is not being cleaned up!! Don't forget about me!");
		fread(res.data, 1, image_size, file);
		fclose(file);

		return res;
	}

	u32 vertex_buffer;
	u32 element_buffer;

	static f32 vertex_buffer_data[] = {
		0.5f, 0.5f, 0.f, 1.f, 0.f, 0.f, 1.f, 1.f,
		0.5f, -0.5f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f,
		-0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f,
		-0.5f, 0.5f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f
	};

	static u32 indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};

	s32 shader_program;
	u32 vertex_array;
	u32 texture;

	void InitGLScene()
	{
		s32 vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		GLCall(glShaderSource(vertex_shader, 1, &vertexShaderSource, NULL));
		GLCall(glCompileShader(vertex_shader));

		s32 success;
		char info_log[512];
		GLCall(glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success));
		if (!success)
		{
			GLCall(glGetShaderInfoLog(vertex_shader, 512, NULL, info_log));
			MARS_CORE_ERROR(info_log);
		}

		s32 fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		GLCall(glShaderSource(fragment_shader, 1, &fragmentShaderSource, NULL));
		GLCall(glCompileShader(fragment_shader));

		GLCall(glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success));
		if (!success)
		{
			GLCall(glGetShaderInfoLog(vertex_shader, 512, NULL, info_log));
			MARS_CORE_ERROR(info_log);
		}

		shader_program = glCreateProgram();
		GLCall(glAttachShader(shader_program, vertex_shader));
		GLCall(glAttachShader(shader_program, fragment_shader));
		GLCall(glLinkProgram(shader_program));

		GLCall(glGetProgramiv(shader_program, GL_LINK_STATUS, &success));
		if (!success)
		{
			GLCall(glGetProgramInfoLog(shader_program, 512, NULL, info_log));
			MARS_CORE_ERROR(info_log);
		}

		GLCall(glDeleteShader(vertex_shader));
		GLCall(glDeleteShader(fragment_shader));

		GLCall(glGenVertexArrays(1, &vertex_array));
		GLCall(glGenBuffers(1, &vertex_buffer));
		GLCall(glGenBuffers(1, &element_buffer));

		GLCall(glBindVertexArray(vertex_array));

		GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer));
		GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW));

		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)0));
		GLCall(glEnableVertexAttribArray(0));

		GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(3 * sizeof(f32))));
		GLCall(glEnableVertexAttribArray(1));

		GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(6 * sizeof(f32))));
		GLCall(glEnableVertexAttribArray(2));

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		GLTexture wall = LoadBMP("C://MarsEngine//Mars//res//wall.bmp");
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wall.width, wall.height, 0, GL_BGR, GL_UNSIGNED_BYTE, wall.data);
		glGenerateMipmap(GL_TEXTURE_2D);
		free(wall.data);
	}

	void RenderScene()
	{
		GLCall(glClearColor(1.f, 0.f, 0.f, 1.f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		glBindTexture(GL_TEXTURE_2D, texture);

		GLCall(glUseProgram(shader_program));
		GLCall(glBindVertexArray(vertex_array));
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

		SwapBuffers(GetDC(game_state.hwnd));
	}
}
