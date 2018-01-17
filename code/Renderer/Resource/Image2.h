#pragma once

#include "TextureBase.h"

enum class Orientation
{
	R0,
	R90,
	R180,
	R270,
	HFlip,
	HFlipR90,
	HFlipR270,
	VFlip,
};

struct ImageMip
{
	TextureFormat::Enum m_format;
	uint32_t m_width;
	uint32_t m_height;
	uint32_t m_depth;
	uint32_t m_blockSize;
	uint32_t m_size;
	uint8_t  m_bpp;
	bool     m_hasAlpha;
	const uint8_t *m_data;
};

class Image2 : public Noncopyable
{
public:
	bool Init(const std::string &filename);

	const std::vector<uint8_t>& GetData() const { return m_data; }

	uint32_t GetWidth() const { return m_width; }
	uint32_t GetHeight() const { return m_height; }

private:
	void imageAlloc(TextureFormat::Enum format, uint16_t width, uint16_t height, uint16_t depth, uint16_t numLayers, bool cubeMap, bool hasMips, const uint8_t *data = NULL);
	bool imageGetRawData(uint16_t _side, uint8_t _lod, uint32_t _size, ImageMip &_mip);

	// Image2 Container:
	std::vector<uint8_t> m_data;
	std::string m_filename;
	TextureFormat::Enum m_format;
	Orientation m_orientation = Orientation::R0;
	uint32_t m_size;
	uint32_t m_offset;
	uint32_t m_width;
	uint32_t m_height;
	uint32_t m_depth;
	uint16_t m_numLayers;
	uint8_t m_numMips;
	bool m_hasAlpha;
	bool m_cubeMap;
	bool m_ktx;
	bool m_ktxLE;
	bool m_srgb;

	bool m_isValid = false;
};