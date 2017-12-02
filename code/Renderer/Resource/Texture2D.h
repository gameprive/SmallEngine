#pragma once

#include "Texture.h"

class Texture2D : Noncopyable
{
public:
	Texture2D(const std::string &filename, bool mipMap = false, DataFormat format = DataFormat::RGBA8);
	Texture2D(uint32_t width, uint32_t height, bool mipMap = false, DataFormat format = DataFormat::RGBA8);
	~Texture2D();

	uint32_t GetWidth() const;
	uint32_t GetHeight() const;

	void SetData(const void *pixelData);
	void GenerateMipmap() const;

	void Bind(uint32_t index) const;

private:
	void init();

	uint32_t m_pixelWidth = 0;
	uint32_t m_pixelHeight = 0;
	int32_t m_levelCount = 0;
	DataFormat m_format;
	GLuint m_textureObject = 0;
};