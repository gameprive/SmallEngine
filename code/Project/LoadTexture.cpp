// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "LoadTexture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

//-----------------------------------------------------------------------
GLuint LoadTexture(const char *name, bool highQualityMipmaps)
{
	GLuint result = 0;

	int width, height, numComponents;
	unsigned char *data = stbi_load(name, &width, &height, &numComponents, 4);

	if ( data )
	{
		// flip texture
		uint8_t *temp = new uint8_t[width * 4];
		for ( int i = 0; i < height / 2; i++ )
		{
			memcpy(temp, &data[i * width * 4], (width * 4));
			memcpy(&data[i * width * 4], &data[(height - i - 1) * width * 4], (width * 4));
			memcpy(&data[(height - i - 1) * width * 4], temp, (width * 4));
		}

		glGenTextures(1, &result);
		glBindTexture(GL_TEXTURE_2D, result);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glGenerateMipmap(GL_TEXTURE_2D);

		if ( highQualityMipmaps )
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}

		stbi_image_free(data);
		delete[] temp;
	}
	else
	{
		Log(LevelLog::Error) << "LoadTexture() could not load " << name;
		exit(1);
	}

	return result;
}
//-----------------------------------------------------------------------