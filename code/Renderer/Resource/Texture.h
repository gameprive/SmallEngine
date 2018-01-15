#pragma once

//--------------------------------------------------------------------
#ifndef _REFACTORING__
//--------------------------------------------------------------------
enum class DataFormat : uint8_t
{
	BC1,
	BC2,
	BC3,
	BC4,
	BC5,
	BC6H,
	BC7, 

	Compressed,

	R1,
	R8,
	R16,
	R16F,
	R32I,
	R32U,
	R32F,
	RG8,
	RG16,
	RG16F,
	RG32I,
	RG32U,
	RG32F,
	RGB32I,
	RGB32U,
	RGB32F,
	RGBA8,
	RGBA16,
	RGBA16F,
	RGBA32I,
	RGBA32U,
	RGBA32F,
	R11G11B10F,

	//Depth,

	//D16,
	//D24S8,
	//D32F
};

inline bool IsCompressedFormat(DataFormat format) { return format < DataFormat::Compressed; }

inline GLenum ToInternalFormat(DataFormat format)
{
	switch ( format )
	{
	case DataFormat::BC1: return GL_RGB;
	case DataFormat::BC2: return GL_RGBA;
	case DataFormat::BC3: return GL_RGBA;
	case DataFormat::BC4: return GL_LUMINANCE;
	case DataFormat::BC5: return GL_LUMINANCE_ALPHA;
	case DataFormat::BC6H: return GL_RGB;
	case DataFormat::BC7: return GL_RGBA;
	
	case DataFormat::R1:
	case DataFormat::R8:
	case DataFormat::R16:
	case DataFormat::R16F: return GL_RED;

	case DataFormat::R32I:
	case DataFormat::R32U: return GL_RED_INTEGER;

	case DataFormat::RG8:
	case DataFormat::RG16:
	case DataFormat::RG16F:
	case DataFormat::RG32F: return GL_RG;
	
	case DataFormat::RG32I:
	case DataFormat::RG32U: return GL_RG_INTEGER;
	
	case DataFormat::RGB32F: return GL_RGB;
	
	case DataFormat::RGB32I:
	case DataFormat::RGB32U: return GL_RGB_INTEGER;
	
	case DataFormat::RGBA8:
	case DataFormat::RGBA16:
	case DataFormat::RGBA16F:
	case DataFormat::RGBA32F: return GL_RGBA;
	
	case DataFormat::RGBA32I:
	case DataFormat::RGBA32U: return GL_RGBA_INTEGER;
	
	case DataFormat::R11G11B10F: return GL_RGB;
	
	//case DataFormat::D16:
	//case DataFormat::D32F: return GL_DEPTH_COMPONENT;
	//
	//case DataFormat::D24S8: return GL_DEPTH24_STENCIL8;
	};
	ParameterFailed("GetInternalFormat");
}

inline GLenum ToInternalType(DataFormat format)
{
	switch ( format )
	{
	case DataFormat::BC6H:
	case DataFormat::R16F:
	case DataFormat::R32F:
	case DataFormat::RG16F:
	case DataFormat::RG32F:
	case DataFormat::RGB32F:
	case DataFormat::RGBA16F:
	case DataFormat::RGBA32F:
	case DataFormat::R11G11B10F:
	/*case DataFormat::D32F:*/ return GL_FLOAT;
	
	case DataFormat::R32I:
	case DataFormat::RG32I:
	case DataFormat::RGB32I:
	case DataFormat::RGBA32I: return GL_INT;
	
	case DataFormat::R32U:
	case DataFormat::RG32U:
	case DataFormat::RGB32U:
	case DataFormat::RGBA32U: return GL_UNSIGNED_INT;
	};
	return GL_UNSIGNED_BYTE;
}

inline GLenum toGLMapDataFormat(DataFormat format)
{
	switch ( format )
	{
	case DataFormat::BC1: return GL_COMPRESSED_RGB_S3TC_DXT1_EXT;			// DXT1
	case DataFormat::BC2: return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;			// DXT3
	case DataFormat::BC3: return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;			// DXT5
	case DataFormat::BC4: return GL_COMPRESSED_LUMINANCE_LATC1_EXT;			// LATC1/ATI1
	case DataFormat::BC5: return GL_COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT;	// LATC2/ATI2
	case DataFormat::BC6H: return GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT;		// BC6H
	case DataFormat::BC7: return GL_COMPRESSED_RGBA_BPTC_UNORM;				// BC7

	case DataFormat::R1: return GL_R;
	case DataFormat::R8: return GL_R8;
	case DataFormat::R16: return GL_R16;
	case DataFormat::R16F: return GL_R16F;
	case DataFormat::R32I: return GL_R32I;
	case DataFormat::R32U: return GL_R32UI;
	case DataFormat::R32F: return GL_R32F;
	case DataFormat::RG8: return GL_RG8;
	case DataFormat::RG16: return GL_RG16;
	case DataFormat::RG16F: return GL_RG16F;
	case DataFormat::RG32I: return GL_RG32I;
	case DataFormat::RG32U: return GL_RG32UI;
	case DataFormat::RG32F: return GL_RG32F;
	case DataFormat::RGB32I: return GL_RGB32I;
	case DataFormat::RGB32U: return GL_RGB32UI;
	case DataFormat::RGB32F: return GL_RGB32F;
	case DataFormat::RGBA8: return GL_RGBA8;
	case DataFormat::RGBA16: return GL_RGBA16;
	case DataFormat::RGBA16F: return GL_RGBA16F;
	case DataFormat::RGBA32I: return GL_RGBA32I;
	case DataFormat::RGBA32U: return GL_RGBA32UI;
	case DataFormat::RGBA32F: return GL_RGBA32F;
	case DataFormat::R11G11B10F: return GL_R11F_G11F_B10F;

	//case DataFormat::D16: return GL_DEPTH_COMPONENT16;
	//case DataFormat::D24S8: return GL_DEPTH24_STENCIL8;
	//case DataFormat::D32F: return GL_DEPTH_COMPONENT32F;
	};
	ParameterFailed("MapDataFormat");
}

inline GLsizei ToInternalStride(DataFormat format)
{
	switch ( format )
	{
	case DataFormat::R8: return 1;
	case DataFormat::R16: return 2;
	case DataFormat::R16F: return 2;
	case DataFormat::R32I: return 4;
	case DataFormat::R32U: return 4;
	case DataFormat::R32F: return 4;
	case DataFormat::RG8: return 2;
	case DataFormat::RG16: return 4;
	case DataFormat::RG16F: return 4;
	case DataFormat::RG32I: return 8;
	case DataFormat::RG32U: return 8;
	case DataFormat::RG32F: return 8;
	case DataFormat::RGB32I: return 12;
	case DataFormat::RGB32U: return 12;
	case DataFormat::RGB32F: return 12;
	case DataFormat::RGBA8: return 4;
	case DataFormat::RGBA16: return 8;
	case DataFormat::RGBA16F: return 8;
	case DataFormat::RGBA32I: return 16;
	case DataFormat::RGBA32U: return 16;
	case DataFormat::RGBA32F: return 16; 
	}
	ParameterFailed("InternalStride");
}

inline uint32_t ComputeMipmapLevelCount(uint32_t width, uint32_t height)
{
	uint32_t levelCount = 1;
	uint32_t size = std::max(width, height);
	while ( size > 1 )
	{
		size = size / 2;
		++levelCount;
	}
	return levelCount;
}

//inline GLenum ToTextureUnitIndex(uint32_t index)
//{
//	return static_cast<GLenum>(GL_TEXTURE0 + index);
//}
//--------------------------------------------------------------------
#endif
//--------------------------------------------------------------------
