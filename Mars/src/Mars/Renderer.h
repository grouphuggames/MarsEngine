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
#include <cstdio>


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

	struct Vertex
	{
		vec3 position;
		vec3 normal;
		vec2 uv;
	};

	bool LoadOBJ(const char* path, MVector<Vertex>& verts)
	{
		MVector<u32> vert_indices, uv_indices, normal_indices;
		MVector<vec3> tmp_verts;
		MVector<vec2> tmp_uvs;
		MVector<vec3> tmp_normals;

		MVector<vec3> out_verts;
		MVector<vec2> out_uvs;
		MVector<vec3> out_normals;

		FILE* file = fopen(path, "r");
		if (!file)
		{
			MARS_CORE_ERROR("Could not open file: ", path);
			return false;
		}

		while (true)
		{
			char header[128];

			int res = fscanf(file, "%s", header);
			if (res == EOF)
			{
				break;
			}

			if (strcmp(header, "v") == 0)
			{
				f32 x, y, z;
				fscanf(file, "%f %f %f", &x, &y, &z);
				tmp_verts.PushBack(vec3(x, y, z));
			}
			else if (strcmp(header, "vt") == 0)
			{
				f32 x, y;
				fscanf(file, "%f %f %f", &x, &y);
				tmp_uvs.PushBack(vec2(x, y));
			}
			else if (strcmp(header, "vn") == 0)
			{
				f32 x, y, z;
				fscanf(file, "%f %f %f", &x, &y, &z);
				tmp_normals.PushBack(vec3(x, y, z));
			}
			else if (strcmp(header, "f") == 0)
			{
				u32 vert_index[3], uv_index[3], normal_index[3];

				s32 matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vert_index[0], &uv_index[0], &normal_index[0], &vert_index[1], &uv_index[1], &normal_index[1], &vert_index[2], &uv_index[2], &normal_index[2]);
				if (matches != 9)
				{
					MARS_CORE_ERROR("File cannot be parsed correctly. Make sure you are exporting with the proper options!!");
					return false;
				}

				vert_indices.PushBack(vert_index[0]);
				vert_indices.PushBack(vert_index[1]);
				vert_indices.PushBack(vert_index[2]);
				uv_indices.PushBack(uv_index[0]);
				uv_indices.PushBack(uv_index[1]);
				uv_indices.PushBack(uv_index[2]);
				normal_indices.PushBack(normal_index[0]);
				normal_indices.PushBack(normal_index[1]);
				normal_indices.PushBack(normal_index[2]);
			}
		}

		for (u32 i = 0; i < vert_indices.Size(); i++)
		{
			u32 vert_index = vert_indices[i];
			vec3 vert = tmp_verts[vert_index - 1];
			out_verts.PushBack(vert);
		}

		for (u32 i = 0; i < uv_indices.Size(); i++)
		{
			u32 uv_index = uv_indices[i];
			vec2 uv = tmp_uvs[uv_index - 1];
			out_uvs.PushBack(uv);
		}

		for (u32 i = 0; i < normal_indices.Size(); i++)
		{
			u32 normal_index = normal_indices[i];
			vec3 normal = tmp_normals[normal_index - 1];
			out_normals.PushBack(normal);
		}

		s32 total_verts = Max(Max(out_verts.Size(), out_uvs.Size()), out_normals.Size());	// set this equal to the max value between out_verts.Size(), out_uvs.Size(), and out_normals.Size()
		for (s32 i = 0; i < total_verts; i++)
		{
			Vertex vertex;
			vertex.position = out_verts[i];
			vertex.normal = out_normals[i];
			vertex.uv = out_uvs[i];
			verts.PushBack(vertex);
		}
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

	s32 shader_program;
	u32 vertex_array;
	u32 texture;

	mat4 view;

	MVector<Vertex> monkey_verts;
	MVector<u32> indices;

	void InitGLScene()
	{
		CreateCamera();

		s32 vertex_shader = CreateShader(GL_VERTEX_SHADER, "C://MarsEngine//Mars//res//test_vs.vs");
		s32 fragment_shader = CreateShader(GL_FRAGMENT_SHADER, "C://MarsEngine//Mars//res//test_fs.fs");

		shader_program = CreateShaderProgram(vertex_shader, fragment_shader);

		GLCall(glDeleteShader(vertex_shader));
		GLCall(glDeleteShader(fragment_shader));

		// load obj here
		LoadOBJ("C://MarsEngine//Mars//res//crazymonkey.obj", monkey_verts);
		



		// this algorithm needs to be moved into LoadOBJ to prevent duplicate vertices from even being pushed back into the vertex vector
		struct IndexPair
		{
			u32 index;
			vec3 value;
		};

		MVector<IndexPair> other_vals;

		for (u32 i = 0; i < monkey_verts.Size(); i++)
		{
			if (i == 0)		// for better branch prediction (and hopefully performance), move this out of the for loop since this only happens when i = 0, that removes the if from the loop
			{
				IndexPair tmp;
				tmp.index = i;
				tmp.value = monkey_verts[i].position;
				other_vals.PushBack(tmp);
				indices.PushBack(i);
				continue;
			}

			for (u32 j = 0; j < other_vals.Size(); j++)
			{
				if (monkey_verts[i].position == other_vals[j].value)
				{
					indices.PushBack(j);
				}
				else
				{
					indices.PushBack(i);
				}
			}
		}





		GLCall(glGenVertexArrays(1, &vertex_array));
		GLCall(glGenBuffers(1, &vertex_buffer));
		GLCall(glGenBuffers(1, &element_buffer));

		GLCall(glBindVertexArray(vertex_array));

		GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer));
		GLCall(glBufferData(GL_ARRAY_BUFFER, monkey_verts.Size() * sizeof(Vertex), &monkey_verts[0], GL_STATIC_DRAW));

		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.Size() * sizeof(u32), &indices[0], GL_STATIC_DRAW));

		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0));

		GLCall(glEnableVertexAttribArray(1));
		GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec3))));

		GLCall(glEnableVertexAttribArray(2));
		GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(vec3))));

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

		model *= mat4::Rotate(vec3(1.f, 0.5f, 0.1f), game_state.elapsed_time * ToRadians(70.f));
		view = mat4::LookAtLH(camera_data.camera_position, camera_data.camera_position + camera_data.camera_front, camera_data.camera_up);
		projection *= mat4::PerspectiveFovLH(ToRadians(45.f), (f32)game_state.window_width / (f32)game_state.window_height, 0.1f, 100.f);

		glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, model.GetData());
		glUniformMatrix4fv(glGetUniformLocation(shader_program, "view"), 1, GL_FALSE, view.GetData());
		glUniformMatrix4fv(glGetUniformLocation(shader_program, "projection"), 1, GL_FALSE, projection.GetData());

		GLCall(glBindVertexArray(vertex_array));
		//glDrawArrays(GL_TRIANGLES, 0, monkey_verts.Size());
		glDrawElements(GL_TRIANGLES, indices.Size(), GL_UNSIGNED_INT, (void*)0);

		SwapBuffers(GetDC(game_state.hwnd));
	}
}
