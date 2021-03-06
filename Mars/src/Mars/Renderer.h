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
#include "Texture.h"
#include "Model.h"


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

	mat4 view;

	u32 monkey_vertex_buffer;
	u32 monkey_element_buffer;
		
	s32 monkey_shader_program;
	u32 monkey_vertex_array;

	s32 monkey_vertex_shader;
	s32 monkey_fragment_shader;

	MVector<Vertex> monkey_verts;
	MVector<u32> monkey_indices;
	MVector<u32> monkey_textures;

	u32 sphere_vertex_buffer;
	u32 sphere_element_buffer;

	s32 sphere_shader_program;
	u32 sphere_vertex_array;

	MVector<Vertex> sphere_verts;
	MVector<u32> sphere_indices;

	void InitGLScene()
	{
		CreateCamera();

		monkey_vertex_shader = CreateShader(GL_VERTEX_SHADER, "C://MarsEngine//Mars//res//model.vs");		// absolute path is a no-no
		monkey_fragment_shader = CreateShader(GL_FRAGMENT_SHADER, "C://MarsEngine//Mars//res//model.fs");	// absolute path is a no-no

		monkey_shader_program = CreateShaderProgram(monkey_vertex_shader, monkey_fragment_shader);

		GLCall(glDeleteShader(monkey_vertex_shader));
		GLCall(glDeleteShader(monkey_fragment_shader));

		// load obj here
		LoadOBJ("C://MarsEngine//Mars//res//crazymonkey.obj", monkey_verts, monkey_indices);								// absolute path is a no-no

		GLCall(glGenVertexArrays(1, &monkey_vertex_array));
		GLCall(glGenBuffers(1, &monkey_vertex_buffer));
		GLCall(glGenBuffers(1, &monkey_element_buffer));

		GLCall(glBindVertexArray(monkey_vertex_array));

		GLCall(glBindBuffer(GL_ARRAY_BUFFER, monkey_vertex_buffer));
		GLCall(glBufferData(GL_ARRAY_BUFFER, monkey_verts.Size() * sizeof(Vertex), &monkey_verts[0], GL_STATIC_DRAW));

		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, monkey_element_buffer));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, monkey_indices.Size() * sizeof(u32), &monkey_indices[0], GL_STATIC_DRAW));

		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0));

		GLCall(glEnableVertexAttribArray(1));
		GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec3))));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(vec3)));
		
		monkey_textures.PushBack(CreateGLTexture("C://MarsEngine//Mars//res//wall.bmp"));					// absolute path is a no-no

		s32 sphere_vertex_shader = CreateShader(GL_VERTEX_SHADER, "C://MarsEngine//Mars//res//light.vs");		// absolute path is a no-no
		s32 sphere_fragment_shader = CreateShader(GL_FRAGMENT_SHADER, "C://MarsEngine//Mars//res//light.fs");	// absolute path is a no-no

		sphere_shader_program = CreateShaderProgram(sphere_vertex_shader, sphere_fragment_shader);

		GLCall(glDeleteShader(sphere_vertex_shader));
		GLCall(glDeleteShader(sphere_fragment_shader));

		// load obj here
		LoadOBJ("C://MarsEngine//Mars//res//sphere.obj", sphere_verts, sphere_indices);								// absolute path is a no-no

		GLCall(glGenVertexArrays(1, &sphere_vertex_array));
		GLCall(glGenBuffers(1, &sphere_vertex_buffer));
		GLCall(glGenBuffers(1, &sphere_element_buffer));

		GLCall(glBindVertexArray(sphere_vertex_array));

		GLCall(glBindBuffer(GL_ARRAY_BUFFER, sphere_vertex_buffer));
		GLCall(glBufferData(GL_ARRAY_BUFFER, sphere_verts.Size() * sizeof(Vertex), &sphere_verts[0], GL_STATIC_DRAW));

		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_element_buffer));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere_indices.Size() * sizeof(u32), &sphere_indices[0], GL_STATIC_DRAW));

		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0));
	}

	void RenderScene()
	{
		f32 current_frame = game_state.elapsed_time;
		game_state.current_frame_time = current_frame - game_state.last_frame_time;
		game_state.last_frame_time = current_frame;

		GLCall(glClearColor(0.05f, 0.05f, 0.05f, 1.f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		glBindTexture(GL_TEXTURE_2D, monkey_textures[0]);

		GLCall(glUseProgram(monkey_shader_program));

		SetUniformVec3(monkey_shader_program, "light.position", vec3(5.f, 0.5f, 0.1f));
		SetUniformVec3(monkey_shader_program, "view_position", camera_data.camera_position);
		SetUniformVec3(monkey_shader_program, "light.ambient", vec3(0.05f, 0.05f, 0.05f));
		SetUniformVec3(monkey_shader_program, "light.diffuse", vec3(0.5f, 0.5f, 0.5f));
		SetUniformVec3(monkey_shader_program, "light.specular", vec3(1.f, 1.f, 1.f));
		SetUniformVec3(monkey_shader_program, "material.specular", vec3(0.5f, 0.5f, 0.5f));
		SetUniformFloat(monkey_shader_program, "material.shininess", 64.f);
		SetUniformInt(monkey_shader_program, "material.diffuse", 0);

		mat4 projection(1.f);

		mat4 monkey_model;

		monkey_model = mat4::Rotate(vec3(1.f, 0.5f, 0.1f), game_state.elapsed_time * ToRadians(70.f));
		view = mat4::LookAtLH(camera_data.camera_position, camera_data.camera_position + camera_data.camera_front, camera_data.camera_up);
		projection *= mat4::PerspectiveFovLH(ToRadians(45.f), (f32)game_state.window_width / (f32)game_state.window_height, 0.1f, 100.f);

		SetUniformMat4(monkey_shader_program, "model", monkey_model);
		SetUniformMat4(monkey_shader_program, "view", view);
		SetUniformMat4(monkey_shader_program, "projection", projection);

		GLCall(glBindVertexArray(monkey_vertex_array));
		glDrawElements(GL_TRIANGLES, monkey_indices.Size(), GL_UNSIGNED_INT, (void*)0);

		glBindTexture(GL_TEXTURE_2D, 0);

		GLCall(glUseProgram(sphere_shader_program));

		mat4 sphere_model = mat4::Translate(vec3(5.f, 0.5f, 0.1f));

		SetUniformMat4(sphere_shader_program, "model", sphere_model);
		SetUniformMat4(sphere_shader_program, "view", view);
		SetUniformMat4(sphere_shader_program, "projection", projection);

		GLCall(glBindVertexArray(sphere_vertex_array));
		glDrawElements(GL_TRIANGLES, sphere_indices.Size(), GL_UNSIGNED_INT, (void*)0);

		SwapBuffers(GetDC(game_state.hwnd));
	}
}
