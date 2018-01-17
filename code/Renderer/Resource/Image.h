#pragma once

#include "TextureBase.h"

class Image : public Noncopyable
{
public:
	bool Init(const std::string &filename);

	const std::vector<uint8_t>& GetData() const { return m_data; }
	uint32_t GetWidth() const { return m_width; }
	uint32_t GetHeight() const { return m_height; }
	TextureFormat::Enum GetFormat() const { return m_format; }

private:
	bool loadSTB(const FileReader &fileData);

	std::string m_filename;
	uint32_t m_width = 0;
	uint32_t m_height = 0;
	std::vector<uint8_t> m_data;

	TextureFormat::Enum m_format;
};