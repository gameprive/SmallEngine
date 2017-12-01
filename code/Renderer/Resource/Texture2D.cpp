// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "Texture2D.h"
#include "Image.h"
#include "System/RenderDevice.h"

////-----------------------------------------------------------------------
//int toBytesPerBlock(DataFormat format)
//{
//	switch ( format )
//	{
//	case DataFormat::A8_UNorm:
//	case DataFormat::R8_UNorm:
//	case DataFormat::BlockComp1_UNorm:
//		return 1;
//	case DataFormat::R8G8_UNorm:
//	case DataFormat::BlockComp2_UNorm:
//	case DataFormat::BlockComp3_UNorm:
//		return 2;
//	case DataFormat::R8G8B8A8_UNorm:
//	case DataFormat::B8G8R8A8_UNorm:
//	case DataFormat::R10G10B10A2_UNorm:
//	case DataFormat::R11G11B10_Float:
//	case DataFormat::R16G16_Float:
//	case DataFormat::R32_Float:
//		return 4;
//	case DataFormat::R16G16B16A16_Float:
//		return 8;
//	case DataFormat::R32G32B32A32_Float:
//		return 16;
//	};
//	return 1;
//}
//-----------------------------------------------------------------------
//GLenum toFormatComponents(DataFormat format)
//{
//	switch ( format )
//	{
//	case DataFormat::R8G8B8A8_UNorm:
//	case DataFormat::R16G16B16A16_Float:
//	case DataFormat::R32G32B32A32_Float:
//	case DataFormat::R10G10B10A2_UNorm: return GL_RGBA;
//	case DataFormat::R11G11B10_Float: return GL_RGB;
//	case DataFormat::R8G8_UNorm:
//	case DataFormat::R16G16_Float: return GL_RG;
//	case DataFormat::R8_UNorm:
//	case DataFormat::R32_Float:
//	case DataFormat::A8_UNorm: return GL_RED;
//	case DataFormat::B8G8R8A8_UNorm: return GL_BGRA;
//	case DataFormat::BlockComp1_UNorm:
//	case DataFormat::BlockComp2_UNorm:
//	case DataFormat::BlockComp3_UNorm: return GL_RED;
//	};
//	return GL_RED;
//}
////-----------------------------------------------------------------------
//GLenum toPixelFundamentalType(DataFormat format)
//{
//	switch ( format )
//	{
//	case DataFormat::A8_UNorm:
//	case DataFormat::R8_UNorm:
//	case DataFormat::R8G8_UNorm:
//	case DataFormat::R8G8B8A8_UNorm:
//	case DataFormat::B8G8R8A8_UNorm: return GL_UNSIGNED_BYTE;
//	case DataFormat::R11G11B10_Float:
//	case DataFormat::R32G32B32A32_Float:
//	case DataFormat::R16G16_Float:
//	case DataFormat::R16G16B16A16_Float:
//	case DataFormat::R32_Float: return GL_FLOAT;
//	case DataFormat::R10G10B10A2_UNorm: return GL_UNSIGNED_INT_10_10_10_2;
//	case DataFormat::BlockComp1_UNorm:
//	case DataFormat::BlockComp2_UNorm:
//	case DataFormat::BlockComp3_UNorm: return GL_UNSIGNED_BYTE;
//	};
//	return GL_UNSIGNED_BYTE;
//}
////-----------------------------------------------------------------------
//GLsizei mipmapImageDataBytes(GLsizei width, GLsizei height, GLsizei bytesPerBlock)
//{
//	return width * height * bytesPerBlock;
//}
////-----------------------------------------------------------------------
//void setPixelDataTexture2DCompressedGL4(uint32_t pixelWidth, uint32_t pixelHeight, int32_t levelCount, DataFormat format, const void *pixelData)
//{
//	const auto internalFormat = toInternalFormatGL4(format);
//	const auto bytesPerBlock = toBytesPerBlock(format);
//
//	size_t startOffset = 0;
//	GLsizei mipMapPixelWidth = pixelWidth;
//	GLsizei mipMapPixelHeight = pixelHeight;
//	for ( GLint mipmapLevel = 0; mipmapLevel < levelCount; ++mipmapLevel )
//	{
//		Assert(mipMapPixelWidth > 0);
//		Assert(mipMapPixelHeight > 0);
//
//		const GLsizei strideBytesPerMipmap = mipmapImageDataBytes(mipMapPixelWidth, mipMapPixelHeight, bytesPerBlock);
//
//		glCompressedTexSubImage2D(GL_TEXTURE_2D,
//			mipmapLevel, 0, 0,
//			mipMapPixelWidth,
//			mipMapPixelHeight,
//			internalFormat,
//			strideBytesPerMipmap,
//			reinterpret_cast<const uint8_t*>(pixelData)+startOffset);
//
//		startOffset += strideBytesPerMipmap;
//
//		mipMapPixelWidth = std::max((mipMapPixelWidth >> 1), 1);
//		mipMapPixelHeight = std::max((mipMapPixelHeight >> 1), 1);
//	}
//}
//-----------------------------------------------------------------------
void setPixelDataTexture2DGL4(uint32_t pixelWidth, uint32_t pixelHeight, int32_t levelCount, DataFormat format, const void *pixelData)
{
	const auto formatComponents = toFormatComponents(format);
	const auto pixelFundamentalType = toPixelFundamentalType(format);
	const auto bytesPerBlock = toBytesPerBlock(format);

	size_t startOffset = 0;
	GLsizei mipMapPixelWidth = pixelWidth;
	GLsizei mipMapPixelHeight = pixelHeight;
	for ( GLint mipmapLevel = 0; mipmapLevel < levelCount; ++mipmapLevel )
	{
		const GLsizei strideBytesPerMipmap = mipmapImageDataBytes(mipMapPixelWidth, mipMapPixelHeight, bytesPerBlock);

		glTexSubImage2D(GL_TEXTURE_2D,
			mipmapLevel, 0, 0,
			mipMapPixelWidth,
			mipMapPixelHeight,
			formatComponents,
			pixelFundamentalType,
			reinterpret_cast<const uint8_t*>(pixelData)+startOffset);

		startOffset += strideBytesPerMipmap;

		mipMapPixelWidth = std::max((mipMapPixelWidth >> 1), 1);
		mipMapPixelHeight = std::max((mipMapPixelHeight >> 1), 1);
	}
}
//-----------------------------------------------------------------------
//uint32_t mipmapImageDataBytes(uint32_t pixelWidth, uint32_t pixelHeight, uint32_t bytesPerBlock)
//{
//	return pixelWidth * pixelHeight * bytesPerBlock;
//}
////-----------------------------------------------------------------------
//int32_t computeTextureSizeInBytes(uint32_t pixelWidth, uint32_t pixelHeight, int32_t levelCount, DataFormat format)
//{
//	Assert(pixelWidth > 0);
//	Assert(pixelHeight > 0);
//	Assert(levelCount > 0);
//
//	const auto bytesPerBlock = toBytesPerBlock(format);
//
//	int32_t sizeInBytes = 0;
//	int32_t mipMapWidth = pixelWidth;
//	int32_t mipMapHeight = pixelHeight;
//	for ( int mipmapLevel = 0; mipmapLevel < levelCount; ++mipmapLevel )
//	{
//		sizeInBytes += mipmapImageDataBytes(mipMapWidth, mipMapHeight, bytesPerBlock);
//		mipMapWidth = std::max((mipMapWidth >> 1), 1);
//		mipMapHeight = std::max((mipMapHeight >> 1), 1);
//	}
//
//	return sizeInBytes;
//}
//-----------------------------------------------------------------------
//Texture2D::Texture2D(const std::string &filename, bool mipMap, DataFormat format)
/*	: m_levelCount(mipMap ? computeMipmapLevelCount(m_pixelWidth, m_pixelHeight) : 1)
	, m_format(format)*/
//{
/*	Assert(m_levelCount > 0);

	Image img;
	img.Init(filename);
	m_pixelWidth = img.GetWidth();
	m_pixelHeight = img.GetHeight();
	Assert(m_pixelWidth > 0);
	Assert(m_pixelHeight > 0);



	glGenTextures(1, &m_textureObject);
	Assert(m_textureObject);
	glBindTexture(GL_TEXTURE_2D, m_textureObject);

	Assert(m_pixelWidth > 0);
	Assert(m_pixelHeight > 0);
	Assert(m_levelCount > 0);
	Assert(m_levelCount <= std::numeric_limits<GLsizei>::max());

	glTexStorage2D(GL_TEXTURE_2D, m_levelCount, toInternalFormatGL4(format), m_pixelWidth, m_pixelHeight);

	// Set mipmap levels
	Assert(m_levelCount > 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, m_levelCount - 1);
	

	SetData(img.GetData().data());*/
//}
//-----------------------------------------------------------------------
Texture2D::Texture2D(uint32_t pixelWidth, uint32_t pixelHeight, bool mipMap, DataFormat format)
	: m_pixelWidth(pixelWidth)
	, m_pixelHeight(pixelHeight)
	, m_levelCount(mipMap ? ComputeMipmapLevelCount(m_pixelWidth, m_pixelHeight) : 1)
	, m_format(format)
{
	Assert(m_pixelWidth > 0);
	Assert(m_pixelHeight > 0);
	Assert(m_levelCount > 0);
	Assert(m_levelCount <= std::numeric_limits<GLsizei>::max());

	glGenTextures(1, &m_textureObject);
	Assert(m_textureObject);
	glBindTexture(GL_TEXTURE_2D, m_textureObject);

	glInternalFormat = ToInternalFormat(format);
	glType = ToInternalType(format);
	
	glTexStorage2D(GL_TEXTURE_2D, m_levelCount, toGLMapDataFormat(format), m_pixelWidth, m_pixelHeight);

	// Set mipmap levels
	Assert(m_levelCount > 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, m_levelCount - 1);
}
//-----------------------------------------------------------------------
Texture2D::~Texture2D()
{
	if ( m_textureObject ) 
		glDeleteTextures(1, &m_textureObject);
}
//-----------------------------------------------------------------------
uint32_t Texture2D::GetWidth() const
{
	return m_pixelWidth;
}
//-----------------------------------------------------------------------
uint32_t Texture2D::GetHeight() const
{
	return m_pixelHeight;
}
//-----------------------------------------------------------------------
int32_t Texture2D::GetLevelCount() const
{
	return m_levelCount;
}
//-----------------------------------------------------------------------
DataFormat Texture2D::GetFormat() const
{
	return m_format;
}
//-----------------------------------------------------------------------
void Texture2D::SetData(const void *pixelData)
{
	Assert(pixelData);
	Assert(m_textureObject);
	glBindTexture(GL_TEXTURE_2D, m_textureObject);

	switch ( m_format )
	{
//	case DataFormat::BlockComp1_UNorm:
//	case DataFormat::BlockComp2_UNorm:
//	case DataFormat::BlockComp3_UNorm:
//		setPixelDataTexture2DCompressedGL4(m_pixelWidth, m_pixelHeight, m_levelCount, m_format, pixelData);
//		break;
	default:
		setPixelDataTexture2D(m_pixelWidth, m_pixelHeight, m_levelCount, m_format, pixelData);
		break;
	}
}
//-----------------------------------------------------------------------
void Texture2D::Bind(uint32_t index) const
{
#if defined(_DEBUG) && !defined(NDEBUG)
	Assert(index < RenderDevice::Get().GetMaxTextureSlot());
#endif
	
	glActiveTexture(ToTextureUnitIndex(index));
	glBindTexture(GL_TEXTURE_2D, m_textureObject);
}
//-----------------------------------------------------------------------
void Texture2D::GenerateMipmap() const
{
	Assert(m_textureObject);
	glBindTexture(GL_TEXTURE_2D, m_textureObject);
	glGenerateMipmap(GL_TEXTURE_2D);
}
//-----------------------------------------------------------------------