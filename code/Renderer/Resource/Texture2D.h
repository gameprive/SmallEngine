#pragma once

#include "Texture.h"

class Texture2D : Noncopyable
{
public:
	//Texture2D(const std::string &filename, bool mipMap = false, DataFormat format = DataFormat::RGBA8);
	Texture2D(uint32_t width, uint32_t height, bool mipMap = false, DataFormat format = DataFormat::RGBA8);
	~Texture2D();

	uint32_t GetWidth() const;
	uint32_t GetHeight() const;
	int32_t GetLevelCount() const;
	DataFormat GetFormat() const;

	void SetData(const void *pixelData);
	void GenerateMipmap() const;

	void Bind(uint32_t index) const;

private:
	uint32_t m_pixelWidth;
	uint32_t m_pixelHeight;
	int32_t m_levelCount;
	DataFormat m_format;

	GLuint m_textureObject = 0;

	// ==> temp
	GLenum glInternalFormat = 0;
	GLenum glType = 0;
};