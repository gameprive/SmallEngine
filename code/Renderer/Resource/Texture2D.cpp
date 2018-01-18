// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "Texture2D.h"
#include "OldImage.h"
#include "Image2.h"
#include "Image.h"
#include "System/RenderDevice.h"

//--------------------------------------------------------------------
inline GLsizei mipmapImageDataBytes(GLsizei width, GLsizei height, GLsizei bytesPerBlock)
{
	return width * height * bytesPerBlock;
}
//--------------------------------------------------------------------
void setPixelDataTexture2DCompressed(uint32_t pixelWidth, uint32_t pixelHeight, int32_t levelCount, DataFormat format, const void *pixelData)
{
	const auto internalFormat = toGLMapDataFormat(format);
	const auto bytesPerBlock = ToInternalStride(format);

	size_t startOffset = 0;
	GLsizei mipMapWidth = pixelWidth;
	GLsizei mipMapHeight = pixelHeight;
	for ( int mipmapLevel = 0; mipmapLevel < levelCount; mipmapLevel++ )
	{
		const auto strideBytesPerMipmap = mipmapImageDataBytes(mipMapWidth, mipMapHeight, bytesPerBlock);
		const uint8_t *data = reinterpret_cast<const uint8_t*>(pixelData)+startOffset;

		glCompressedTexSubImage2D(GL_TEXTURE_2D, mipmapLevel, 0, 0, mipMapWidth, mipMapHeight, internalFormat, strideBytesPerMipmap, data);

		startOffset += strideBytesPerMipmap;
		mipMapWidth = std::max((mipMapWidth >> 1), 1);
		mipMapHeight = std::max((mipMapHeight >> 1), 1);
	}
}
//--------------------------------------------------------------------
void setPixelDataTexture2D(uint32_t pixelWidth, uint32_t pixelHeight, int32_t levelCount, DataFormat format, const void *pixelData)
{
	const auto formatComponents = ToInternalFormat(format);
	const auto pixelFundamentalType = ToInternalType(format);
	const auto bytesPerBlock = ToInternalStride(format);

	GLsizei mipMapWidth = pixelWidth;
	GLsizei mipMapHeight = pixelHeight;
	size_t startOffset = 0;
	for ( int mipmapLevel = 0; mipmapLevel < levelCount; mipmapLevel++ )
	{
		const auto strideBytesPerMipmap = mipmapImageDataBytes(mipMapWidth, mipMapHeight, bytesPerBlock);
		const uint8_t *data = reinterpret_cast<const uint8_t*>(pixelData)+startOffset;

		glTexSubImage2D(GL_TEXTURE_2D, mipmapLevel, 0, 0, mipMapWidth, mipMapHeight, formatComponents, pixelFundamentalType, data);

		startOffset += strideBytesPerMipmap;		
		mipMapWidth = std::max((mipMapWidth >> 1), 1);
		mipMapHeight = std::max((mipMapHeight >> 1), 1);
	}
}
//--------------------------------------------------------------------
Texture2D::Texture2D(const std::string &filename, bool mipMap, DataFormat format)
	: m_levelCount(mipMap ? ComputeMipmapLevelCount(m_pixelWidth, m_pixelHeight) : 1)
	, m_format(format)
{
	OldImage img;
	if ( !img.Init(filename) ) return;
	m_pixelWidth = img.GetWidth();
	m_pixelHeight = img.GetHeight();
		
	init();
	
	SetData(img.GetData().data());
}
//--------------------------------------------------------------------
Texture2D::Texture2D(uint32_t pixelWidth, uint32_t pixelHeight, bool mipMap, DataFormat format)
	: m_pixelWidth(pixelWidth)
	, m_pixelHeight(pixelHeight)
	, m_levelCount(mipMap ? ComputeMipmapLevelCount(m_pixelWidth, m_pixelHeight) : 1)
	, m_format(format)
{
	init();
}
//--------------------------------------------------------------------
Texture2D::Texture2D(uint32_t pixelWidth, uint32_t pixelHeight, int32_t levelCount, DataFormat format)
	: m_pixelWidth(pixelWidth)
	, m_pixelHeight(pixelHeight)
	, m_levelCount(levelCount)
	, m_format(format)
{
	init();
}
//--------------------------------------------------------------------
Texture2D::~Texture2D()
{
	if ( m_textureObject ) 
		glDeleteTextures(1, &m_textureObject);
}
//--------------------------------------------------------------------
uint32_t Texture2D::GetWidth() const
{
	return m_pixelWidth;
}
//--------------------------------------------------------------------
uint32_t Texture2D::GetHeight() const
{
	return m_pixelHeight;
}
//--------------------------------------------------------------------
void Texture2D::SetData(const void *pixelData)
{
	Assert(pixelData);
	Assert(m_textureObject);
	glBindTexture(GL_TEXTURE_2D, m_textureObject);

	switch ( m_format )
	{
	case DataFormat::BC1:
	case DataFormat::BC2:
	case DataFormat::BC3:
		setPixelDataTexture2DCompressed(m_pixelWidth, m_pixelHeight, m_levelCount, m_format, pixelData);
		break;
	default:
		setPixelDataTexture2D(m_pixelWidth, m_pixelHeight, m_levelCount, m_format, pixelData);
		break;
	}
}
//--------------------------------------------------------------------
void Texture2D::Bind(uint32_t unit) const
{
	glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + unit));
	glBindTexture(GL_TEXTURE_2D, m_textureObject);
}
//--------------------------------------------------------------------
void Texture2D::GenerateMipmap() const
{
	Assert(m_textureObject);
	glBindTexture(GL_TEXTURE_2D, m_textureObject);
	glGenerateMipmap(GL_TEXTURE_2D);
}
//--------------------------------------------------------------------
void Texture2D::init()
{
	Assert(m_pixelWidth > 0);
	Assert(m_pixelHeight > 0);
	Assert(m_levelCount > 0);
	Assert(m_levelCount <= std::numeric_limits<GLsizei>::max());

	glGenTextures(1, &m_textureObject);
	Assert(m_textureObject);
	glBindTexture(GL_TEXTURE_2D, m_textureObject);

	glTexStorage2D(GL_TEXTURE_2D, m_levelCount, toGLMapDataFormat(m_format), m_pixelWidth, m_pixelHeight);

	// Set mipmap levels
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, m_levelCount - 1);
}
//--------------------------------------------------------------------
