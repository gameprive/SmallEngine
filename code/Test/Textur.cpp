#include "stdafx.h"
#include "Textur.h"
#include <iostream>
#include "Renderer/Resource/Image.h"

GLuint loadDDS(const char* image_path, int *w, int *h)
{
	// for use this method better way :
	// compress your image, save like .DDS ( with photoshop and nvidia utilite )
	// give address image to this method
	// method returns complete !! OpenGL texture for drawing on screen
	unsigned char header[124];

	// пробуем открыть файл
	FILE *fp;
	fp = fopen(image_path, "rb");
	if ( fp == NULL )
	{
		std::cout << "image not load \n";
		return 0;
	}

	// проверим тип файла
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if ( strncmp(filecode, "DDS ", 4) != 0 )
	{
		std::cout << "return 0; \n";
		fclose(fp);
		return 0;
	}

	// читаем заголовок
	fread(&header, 124, 1, fp);

	unsigned int height = *(unsigned int*)&(header[8]);
	unsigned int width = *(unsigned int*)&(header[12]);
	unsigned int linear_size = *(unsigned int*)&(header[16]);
	unsigned int mipmap_count = *(unsigned int*)&(header[24]);
	unsigned int four_cc = *(unsigned int*)&(header[80]);  // formats

	if ( w != nullptr ) *w = width;
	if ( h != nullptr ) *h = height;

#ifdef _DEBUG
	std::cout << "image: " << image_path << " mipmap_count = " << mipmap_count << std::endl;
#endif // _DEBUG


	unsigned char *buffer;
	unsigned int buff_size = mipmap_count > 1 ? linear_size * 2 : linear_size;
	buffer = (unsigned char*)malloc(buff_size * sizeof(unsigned char));
	fread(buffer, 1, buff_size, fp);
	// закрываем файл
	fclose(fp);

#define FOURCC_DXT1 0x31545844 // "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // "DXT5" in ASCII

	unsigned int components = (four_cc == FOURCC_DXT1) ? 3 : 4;;
	unsigned int format;
	if ( four_cc == FOURCC_DXT1 )
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
	else if ( four_cc == FOURCC_DXT3 )
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
	else if ( four_cc == FOURCC_DXT5 )
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;

#undef FOURCC_DXT1
#undef FOURCC_DXT3
#undef FOURCC_DXT5

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	if ( mipmap_count == 1 ) // if we have only 1 image 0 level ( not mipmap )
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);	// highest resolution
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);	// lowest resolution
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else // we have mipmap
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);	//  highest resolution
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipmap_count);	// lowest resolution
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;;
	unsigned int offset = 0;

	// load mipmap. if we have not mipmap mipmap_count = 1 and be loaded 1 image (0 level)
	for ( unsigned int level = 0; level < mipmap_count && (width || height); level++ )
	{
		unsigned int size = ((width + 3) / 4)*((height + 3) / 4)*blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, size, buffer + offset);

		offset += size;
		width /= 2;
		height /= 2;
	}
	free(buffer);

	return textureID;
}