#pragma once
#include "Core.h"
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "mat4.h"
#include "Debug.h"
#include "GLFunctions.h"
#include "GLShader.h"
#include "Camera.h"
#include "MVector.h"


namespace Mars
{
	// OpenGL & Vulkan

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

		InitGLFunctions();

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

		glEnable(GL_DEPTH_TEST);

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

		res.data = new unsigned char[image_size];
		fread(res.data, 1, image_size, file);
		fclose(file);

		return res;
	}

	u32 CreateTexture(const char* path)
	{
		u32 texture;

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		GLTexture wall = LoadBMP(path);	// still using temporary path. maybe create filesystem for engine
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wall.width, wall.height, 0, GL_BGR, GL_UNSIGNED_BYTE, wall.data);
		glGenerateMipmap(GL_TEXTURE_2D);
		free(wall.data);

		return texture;
	}

	u32 vertex_buffer;
	u32 element_buffer;

	float vertex_buffer_data[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	static u32 indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};

	s32 shader_program;
	u32 vertex_array;
	u32 texture;

	mat4 view;

	void InitGLScene()
	{
		CreateCamera();

		s32 vertex_shader = CreateShader(GL_VERTEX_SHADER, "C://MarsEngine//Mars//res//test_vs.vs");
		s32 fragment_shader = CreateShader(GL_FRAGMENT_SHADER, "C://MarsEngine//Mars//res//test_fs.fs");

		shader_program = CreateShaderProgram(vertex_shader, fragment_shader);

		GLCall(glDeleteShader(vertex_shader));
		GLCall(glDeleteShader(fragment_shader));

		// load obj here

		GLCall(glGenVertexArrays(1, &vertex_array));
		GLCall(glGenBuffers(1, &vertex_buffer));
		GLCall(glGenBuffers(1, &element_buffer));

		GLCall(glBindVertexArray(vertex_array));

		GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer));
		GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW));

		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*)0));
		GLCall(glEnableVertexAttribArray(0));

		GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*)(3 * sizeof(f32))));
		GLCall(glEnableVertexAttribArray(1));

		texture = CreateTexture("C://MarsEngine//Mars//res//wall.bmp");
	}

	void RenderScene()
	{
		f32 current_frame = game_state.elapsed_time;
		game_state.current_frame_time = current_frame - game_state.last_frame_time;
		game_state.last_frame_time = current_frame;

		GLCall(glClearColor(0.2f, 0.2f, 0.2f, 1.f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		glBindTexture(GL_TEXTURE_2D, texture);

		GLCall(glUseProgram(shader_program));
		
		mat4 model(1.f);
		mat4 projection(1.f);

		model *= mat4::Rotate(vec3(0.5f, 1.f, 0.1f), game_state.elapsed_time * ToRadians(50.f));
		view = mat4::LookAtLH(camera_data.camera_position, camera_data.camera_position + camera_data.camera_front, camera_data.camera_up);
		projection *= mat4::PerspectiveFovLH(ToRadians(45.f), (f32)game_state.window_width / (f32)game_state.window_height, 0.1f, 100.f);

		glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, model.GetData());
		glUniformMatrix4fv(glGetUniformLocation(shader_program, "view"), 1, GL_FALSE, view.GetData());
		glUniformMatrix4fv(glGetUniformLocation(shader_program, "projection"), 1, GL_FALSE, projection.GetData());

		GLCall(glBindVertexArray(vertex_array));
		//GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		SwapBuffers(GetDC(game_state.hwnd));
	}
}
