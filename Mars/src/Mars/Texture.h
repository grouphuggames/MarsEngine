#pragma once
#include "Core.h"


namespace Mars
{
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
		if (fread(header, 1, 54, file) != 54 || header[0] != 'B' || header[1] != 'M')
		{
			MARS_CORE_ERROR("File: ", path, " is not a BMP!");
			return GLTexture();
		}

		u32 data_position = *(int*) & (header[0x0A]);
		u32 image_size = *(int*) & (header[0x22]);
		res.width = *(int*) & (header[0x12]);
		res.height = *(int*) & (header[0x16]);

		if (image_size == 0) { image_size = 3 * res.width * res.height; }
		if (data_position == 0) { data_position = 54; }

		res.data = new unsigned char[image_size];
		fread(res.data, 1, image_size, file);
		fclose(file);

		return res;
	}

	u32 CreateGLTexture(const char* path)
	{
		u32 texture;

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		GLTexture tex = LoadBMP(path);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex.width, tex.height, 0, GL_BGR, GL_UNSIGNED_BYTE, tex.data);
		glGenerateMipmap(GL_TEXTURE_2D);
		free(tex.data);

		return texture;
	}
}
