#pragma once
#include "Core.h"
#include "GLFunctions.h"
#include <fstream>
#include <string>


namespace Mars
{
	std::string LoadShaderSource(const char* path)
	{
		std::ifstream file;
		file.open(path);

		std::string shader_source;
		std::string line;

		if (file.is_open())
		{
			while (!file.eof())
			{
				std::getline(file, line);
				line.append("\n");
				shader_source.append(line);
			}
		}

		file.close();

		return shader_source;
	}

	s32 CreateShader(u32 shader_type, const char* path)
	{
		s32 shader = glCreateShader(shader_type);
		std::string tmp_shader_source = LoadShaderSource(path);
		const char* shader_source = tmp_shader_source.c_str();
		glShaderSource(shader, 1, &shader_source, NULL);
		glCompileShader(shader);

		s32 success;
		char info_log[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 512, NULL, info_log);
			MARS_CORE_ERROR(info_log);
			return -1;
		}

		return shader;
	}

	s32 CreateShaderProgram(s32 vertex_shader, s32 fragment_shader)
	{
		s32 shader_program = glCreateProgram();
		glAttachShader(shader_program, vertex_shader);
		glAttachShader(shader_program, fragment_shader);
		glLinkProgram(shader_program);

		s32 success;
		char info_log[512];
		GLCall(glGetProgramiv(shader_program, GL_LINK_STATUS, &success));
		if (!success)
		{
			GLCall(glGetProgramInfoLog(shader_program, 512, NULL, info_log));
			MARS_CORE_ERROR(info_log);
			return -1;
		}

		return shader_program;
	}

	void SetUniformVec3(s32 shader_program, const char* uniform_name, vec3 uniform_value)
	{
		glUniform3f(glGetUniformLocation(shader_program, uniform_name), uniform_value.x(), uniform_value.y(), uniform_value.z());
	}

	void SetUniformFloat(s32 shader_program, const char* uniform_name, f32 uniform_value)
	{
		glUniform1f(glGetUniformLocation(shader_program, uniform_name), uniform_value);
	}

	void SetUniformInt(s32 shader_program, const char* uniform_name, s32 uniform_value)
	{
		glUniform1i(glGetUniformLocation(shader_program, uniform_name), uniform_value);
	}
}
