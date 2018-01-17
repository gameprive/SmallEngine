// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "Image2.h"
#include "Core/IO/FileSystem.h"
//#define STBI_NO_PSD
//#define STBI_NO_HDR
//#define STBI_NO_PIC
//#define STBI_NO_GIF
//#define STBI_NO_PNM
//#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

const float kInvLogNat2 = 1.4426950408889634073599246810019f;
inline float Log2(float a)
{
	return ::logf(a) * kInvLogNat2;
}

struct EncodingType
{
	enum Enum
	{
		Unorm,
		Int,
		Uint,
		Float,
		Snorm,

		Count
	};
};

struct ImageBlockInfo
{
	uint8_t bitsPerPixel;
	uint8_t blockWidth;
	uint8_t blockHeight;
	uint8_t blockSize;
	uint8_t minBlockX;
	uint8_t minBlockY;
	uint8_t depthBits;
	uint8_t stencilBits;
	uint8_t rBits;
	uint8_t gBits;
	uint8_t bBits;
	uint8_t aBits;
	uint8_t encoding;
};

static const ImageBlockInfo s_imageBlockInfo[] =
{
	//  +-------------------------------------------- bits per pixel
	//  |  +----------------------------------------- block width
	//  |  |  +-------------------------------------- block height
	//  |  |  |   +---------------------------------- block size
	//  |  |  |   |  +------------------------------- min blocks x
	//  |  |  |   |  |  +---------------------------- min blocks y
	//  |  |  |   |  |  |   +------------------------ depth bits
	//  |  |  |   |  |  |   |  +--------------------- stencil bits
	//  |  |  |   |  |  |   |  |   +---+---+---+----- r, g, b, a bits
	//  |  |  |   |  |  |   |  |   r   g   b   a  +-- encoding type
	//  |  |  |   |  |  |   |  |   |   |   |   |  |
	{   4, 4, 4,  8, 1, 1,  0, 0,  0,  0,  0,  0, uint8_t(EncodingType::Unorm) }, // BC1
	{   8, 4, 4, 16, 1, 1,  0, 0,  0,  0,  0,  0, uint8_t(EncodingType::Unorm) }, // BC2
	{   8, 4, 4, 16, 1, 1,  0, 0,  0,  0,  0,  0, uint8_t(EncodingType::Unorm) }, // BC3
	{   4, 4, 4,  8, 1, 1,  0, 0,  0,  0,  0,  0, uint8_t(EncodingType::Unorm) }, // BC4
	{   8, 4, 4, 16, 1, 1,  0, 0,  0,  0,  0,  0, uint8_t(EncodingType::Unorm) }, // BC5
	{   8, 4, 4, 16, 1, 1,  0, 0,  0,  0,  0,  0, uint8_t(EncodingType::Float) }, // BC6H
	{   8, 4, 4, 16, 1, 1,  0, 0,  0,  0,  0,  0, uint8_t(EncodingType::Unorm) }, // BC7
	{   4, 4, 4,  8, 1, 1,  0, 0,  0,  0,  0,  0, uint8_t(EncodingType::Unorm) }, // ETC1
	{   4, 4, 4,  8, 1, 1,  0, 0,  0,  0,  0,  0, uint8_t(EncodingType::Unorm) }, // ETC2
	{   8, 4, 4, 16, 1, 1,  0, 0,  0,  0,  0,  0, uint8_t(EncodingType::Unorm) }, // ETC2A
	{   4, 4, 4,  8, 1, 1,  0, 0,  0,  0,  0,  0, uint8_t(EncodingType::Unorm) }, // ETC2A1
	{   2, 8, 4,  8, 2, 2,  0, 0,  0,  0,  0,  0, uint8_t(EncodingType::Unorm) }, // PTC12
	{   4, 4, 4,  8, 2, 2,  0, 0,  0,  0,  0,  0, uint8_t(EncodingType::Unorm) }, // PTC14
	{   2, 8, 4,  8, 2, 2,  0, 0,  0,  0,  0,  0, uint8_t(EncodingType::Unorm) }, // PTC12A
	{   4, 4, 4,  8, 2, 2,  0, 0,  0,  0,  0,  0, uint8_t(EncodingType::Unorm) }, // PTC14A
	{   2, 8, 4,  8, 2, 2,  0, 0,  0,  0,  0,  0, uint8_t(EncodingType::Unorm) }, // PTC22
	{   4, 4, 4,  8, 2, 2,  0, 0,  0,  0,  0,  0, uint8_t(EncodingType::Unorm) }, // PTC24
	{   0, 0, 0,  0, 0, 0,  0, 0,  0,  0,  0,  0, uint8_t(EncodingType::Count) }, // Unknown
	{   1, 8, 1,  1, 1, 1,  0, 0,  1,  0,  0,  0, uint8_t(EncodingType::Unorm) }, // R1
	{   8, 1, 1,  1, 1, 1,  0, 0,  0,  0,  0,  8, uint8_t(EncodingType::Unorm) }, // A8
	{   8, 1, 1,  1, 1, 1,  0, 0,  8,  0,  0,  0, uint8_t(EncodingType::Unorm) }, // R8
	{   8, 1, 1,  1, 1, 1,  0, 0,  8,  0,  0,  0, uint8_t(EncodingType::Int) },   // R8I
	{   8, 1, 1,  1, 1, 1,  0, 0,  8,  0,  0,  0, uint8_t(EncodingType::Uint) },  // R8U
	{   8, 1, 1,  1, 1, 1,  0, 0,  8,  0,  0,  0, uint8_t(EncodingType::Snorm) }, // R8S
	{  16, 1, 1,  2, 1, 1,  0, 0, 16,  0,  0,  0, uint8_t(EncodingType::Unorm) }, // R16
	{  16, 1, 1,  2, 1, 1,  0, 0, 16,  0,  0,  0, uint8_t(EncodingType::Int) },   // R16I
	{  16, 1, 1,  2, 1, 1,  0, 0, 16,  0,  0,  0, uint8_t(EncodingType::Uint) },  // R16U
	{  16, 1, 1,  2, 1, 1,  0, 0, 16,  0,  0,  0, uint8_t(EncodingType::Float) }, // R16F
	{  16, 1, 1,  2, 1, 1,  0, 0, 16,  0,  0,  0, uint8_t(EncodingType::Snorm) }, // R16S
	{  32, 1, 1,  4, 1, 1,  0, 0, 32,  0,  0,  0, uint8_t(EncodingType::Int) },   // R32I
	{  32, 1, 1,  4, 1, 1,  0, 0, 32,  0,  0,  0, uint8_t(EncodingType::Uint) },  // R32U
	{  32, 1, 1,  4, 1, 1,  0, 0, 32,  0,  0,  0, uint8_t(EncodingType::Float) }, // R32F
	{  16, 1, 1,  2, 1, 1,  0, 0,  8,  8,  0,  0, uint8_t(EncodingType::Unorm) }, // RG8
	{  16, 1, 1,  2, 1, 1,  0, 0,  8,  8,  0,  0, uint8_t(EncodingType::Int) },   // RG8I
	{  16, 1, 1,  2, 1, 1,  0, 0,  8,  8,  0,  0, uint8_t(EncodingType::Uint) },  // RG8U
	{  16, 1, 1,  2, 1, 1,  0, 0,  8,  8,  0,  0, uint8_t(EncodingType::Snorm) }, // RG8S
	{  32, 1, 1,  4, 1, 1,  0, 0, 16, 16,  0,  0, uint8_t(EncodingType::Unorm) }, // RG16
	{  32, 1, 1,  4, 1, 1,  0, 0, 16, 16,  0,  0, uint8_t(EncodingType::Int) },   // RG16I
	{  32, 1, 1,  4, 1, 1,  0, 0, 16, 16,  0,  0, uint8_t(EncodingType::Uint) },  // RG16U
	{  32, 1, 1,  4, 1, 1,  0, 0, 16, 16,  0,  0, uint8_t(EncodingType::Float) }, // RG16F
	{  32, 1, 1,  4, 1, 1,  0, 0, 16, 16,  0,  0, uint8_t(EncodingType::Snorm) }, // RG16S
	{  64, 1, 1,  8, 1, 1,  0, 0, 32, 32,  0,  0, uint8_t(EncodingType::Int) },   // RG32I
	{  64, 1, 1,  8, 1, 1,  0, 0, 32, 32,  0,  0, uint8_t(EncodingType::Uint) },  // RG32U
	{  64, 1, 1,  8, 1, 1,  0, 0, 32, 32,  0,  0, uint8_t(EncodingType::Float) }, // RG32F
	{  24, 1, 1,  3, 1, 1,  0, 0,  8,  8,  8,  0, uint8_t(EncodingType::Unorm) }, // RGB8
	{  24, 1, 1,  3, 1, 1,  0, 0,  8,  8,  8,  0, uint8_t(EncodingType::Int) },   // RGB8I
	{  24, 1, 1,  3, 1, 1,  0, 0,  8,  8,  8,  0, uint8_t(EncodingType::Uint) },  // RGB8U
	{  24, 1, 1,  3, 1, 1,  0, 0,  8,  8,  8,  0, uint8_t(EncodingType::Snorm) }, // RGB8S
	{  32, 1, 1,  4, 1, 1,  0, 0,  9,  9,  9,  5, uint8_t(EncodingType::Float) }, // RGB9E5F
	{  32, 1, 1,  4, 1, 1,  0, 0,  8,  8,  8,  8, uint8_t(EncodingType::Unorm) }, // BGRA8
	{  32, 1, 1,  4, 1, 1,  0, 0,  8,  8,  8,  8, uint8_t(EncodingType::Unorm) }, // RGBA8
	{  32, 1, 1,  4, 1, 1,  0, 0,  8,  8,  8,  8, uint8_t(EncodingType::Int) },   // RGBA8I
	{  32, 1, 1,  4, 1, 1,  0, 0,  8,  8,  8,  8, uint8_t(EncodingType::Uint) },  // RGBA8U
	{  32, 1, 1,  4, 1, 1,  0, 0,  8,  8,  8,  8, uint8_t(EncodingType::Snorm) }, // RGBA8S
	{  64, 1, 1,  8, 1, 1,  0, 0, 16, 16, 16, 16, uint8_t(EncodingType::Unorm) }, // RGBA16
	{  64, 1, 1,  8, 1, 1,  0, 0, 16, 16, 16, 16, uint8_t(EncodingType::Int) },   // RGBA16I
	{  64, 1, 1,  8, 1, 1,  0, 0, 16, 16, 16, 16, uint8_t(EncodingType::Uint) },  // RGBA16U
	{  64, 1, 1,  8, 1, 1,  0, 0, 16, 16, 16, 16, uint8_t(EncodingType::Float) }, // RGBA16F
	{  64, 1, 1,  8, 1, 1,  0, 0, 16, 16, 16, 16, uint8_t(EncodingType::Snorm) }, // RGBA16S
	{ 128, 1, 1, 16, 1, 1,  0, 0, 32, 32, 32, 32, uint8_t(EncodingType::Int) },   // RGBA32I
	{ 128, 1, 1, 16, 1, 1,  0, 0, 32, 32, 32, 32, uint8_t(EncodingType::Uint) },  // RGBA32U
	{ 128, 1, 1, 16, 1, 1,  0, 0, 32, 32, 32, 32, uint8_t(EncodingType::Float) }, // RGBA32F
	{  16, 1, 1,  2, 1, 1,  0, 0,  5,  6,  5,  0, uint8_t(EncodingType::Unorm) }, // R5G6B5
	{  16, 1, 1,  2, 1, 1,  0, 0,  4,  4,  4,  4, uint8_t(EncodingType::Unorm) }, // RGBA4
	{  16, 1, 1,  2, 1, 1,  0, 0,  5,  5,  5,  1, uint8_t(EncodingType::Unorm) }, // RGB5A1
	{  32, 1, 1,  4, 1, 1,  0, 0, 10, 10, 10,  2, uint8_t(EncodingType::Unorm) }, // RGB10A2
	{  32, 1, 1,  4, 1, 1,  0, 0, 11, 11, 10,  0, uint8_t(EncodingType::Unorm) }, // RG11B10F
	{   0, 0, 0,  0, 0, 0,  0, 0,  0,  0,  0,  0, uint8_t(EncodingType::Count) }, // UnknownDepth
	{  16, 1, 1,  2, 1, 1, 16, 0,  0,  0,  0,  0, uint8_t(EncodingType::Unorm) }, // D16
	{  24, 1, 1,  3, 1, 1, 24, 0,  0,  0,  0,  0, uint8_t(EncodingType::Unorm) }, // D24
	{  32, 1, 1,  4, 1, 1, 24, 8,  0,  0,  0,  0, uint8_t(EncodingType::Unorm) }, // D24S8
	{  32, 1, 1,  4, 1, 1, 32, 0,  0,  0,  0,  0, uint8_t(EncodingType::Unorm) }, // D32
	{  16, 1, 1,  2, 1, 1, 16, 0,  0,  0,  0,  0, uint8_t(EncodingType::Float) }, // D16F
	{  24, 1, 1,  3, 1, 1, 24, 0,  0,  0,  0,  0, uint8_t(EncodingType::Float) }, // D24F
	{  32, 1, 1,  4, 1, 1, 32, 0,  0,  0,  0,  0, uint8_t(EncodingType::Float) }, // D32F
	{   8, 1, 1,  1, 1, 1,  0, 8,  0,  0,  0,  0, uint8_t(EncodingType::Unorm) }, // D0S8
};
inline const ImageBlockInfo& getBlockInfo(TextureFormat::Enum format)
{
	return s_imageBlockInfo[format];
}
inline uint8_t calcNumMips(bool hasMips, uint16_t width, uint16_t height, uint16_t depth = 1)
{
	if ( hasMips )
	{
		const uint32_t max = std::max(std::max(width, height), depth);
		const uint32_t num = 1 + uint32_t(Log2(float(max)));
		return uint8_t(num);
	}

	return 1;
}
uint8_t imageGetNumMips(TextureFormat::Enum format, uint16_t width, uint16_t height, uint16_t depth)
{
	const ImageBlockInfo& blockInfo = getBlockInfo(format);
	const uint16_t blockWidth = blockInfo.blockWidth;
	const uint16_t blockHeight = blockInfo.blockHeight;
	const uint16_t minBlockX = blockInfo.minBlockX;
	const uint16_t minBlockY = blockInfo.minBlockY;

	width = std::max(blockWidth * minBlockX, ((width + blockWidth - 1) / blockWidth) * blockWidth);
	height = std::max(blockHeight * minBlockY, ((height + blockHeight - 1) / blockHeight) * blockHeight);
	depth = std::max(1ui16, depth);
	
	return calcNumMips(true, width, height, depth);
}
uint32_t imageGetSize(/*TextureInfo* _info,*/ uint16_t _width, uint16_t _height, uint16_t _depth, bool _cubeMap, bool _hasMips, uint16_t _numLayers, TextureFormat::Enum _format)
{
	const ImageBlockInfo &blockInfo = getBlockInfo(_format);
	const uint8_t bpp = blockInfo.bitsPerPixel;
	const uint16_t blockWidth = blockInfo.blockWidth;
	const uint16_t blockHeight = blockInfo.blockHeight;
	const uint16_t minBlockX = blockInfo.minBlockX;
	const uint16_t minBlockY = blockInfo.minBlockY;

	_width = std::max(blockWidth  * minBlockX, ((_width + blockWidth - 1) / blockWidth)*blockWidth);
	_height = std::max(blockHeight * minBlockY, ((_height + blockHeight - 1) / blockHeight)*blockHeight);
	_depth = std::max(1ui16, _depth);
	const uint8_t  numMips = calcNumMips(_hasMips, _width, _height, _depth);
	const uint32_t sides = _cubeMap ? 6 : 1;

	uint32_t width = _width;
	uint32_t height = _height;
	uint32_t depth = _depth;
	uint32_t size = 0;

	for ( uint32_t lod = 0; lod < numMips; ++lod )
	{
		width = std::max((uint32_t)blockWidth  * minBlockX, ((width + blockWidth - 1) / blockWidth)*blockWidth);
		height = std::max((uint32_t)blockHeight * minBlockY, ((height + blockHeight - 1) / blockHeight)*blockHeight);
		depth = std::max(1u, depth);

		size += uint32_t(uint64_t(width*height*depth)*bpp / 8 * sides);

		width >>= 1;
		height >>= 1;
		depth >>= 1;
	}

	size *= _numLayers;

	//if ( NULL != _info )
	//{
	//	_info->format = _format;
	//	_info->width = _width;
	//	_info->height = _height;
	//	_info->depth = _depth;
	//	_info->numMips = numMips;
	//	_info->numLayers = _numLayers;
	//	_info->cubeMap = _cubeMap;
	//	_info->storageSize = size;
	//	_info->bitsPerPixel = bpp;
	//}

	return size;
}

//--------------------------------------------------------------------
// DDS
#define DDS_MAGIC 0x20534444 // little-endian
#define DDS_HEADER_SIZE 124

#define DDS_DXT1 MAKEFOURCC('D', 'X', 'T', '1')
#define DDS_DXT2 MAKEFOURCC('D', 'X', 'T', '2')
#define DDS_DXT3 MAKEFOURCC('D', 'X', 'T', '3')
#define DDS_DXT4 MAKEFOURCC('D', 'X', 'T', '4')
#define DDS_DXT5 MAKEFOURCC('D', 'X', 'T', '5')
#define DDS_ATI1 MAKEFOURCC('A', 'T', 'I', '1')
#define DDS_BC4U MAKEFOURCC('B', 'C', '4', 'U')
#define DDS_ATI2 MAKEFOURCC('A', 'T', 'I', '2')
#define DDS_BC5U MAKEFOURCC('B', 'C', '5', 'U')
#define DDS_DX10 MAKEFOURCC('D', 'X', '1', '0')

#define DDS_A8R8G8B8       21
#define DDS_R5G6B5         23
#define DDS_A1R5G5B5       25
#define DDS_A4R4G4B4       26
#define DDS_A2B10G10R10    31
#define DDS_G16R16         34
#define DDS_A2R10G10B10    35
#define DDS_A16B16G16R16   36
#define DDS_A8L8           51
#define DDS_R16F           111
#define DDS_G16R16F        112
#define DDS_A16B16G16R16F  113
#define DDS_R32F           114
#define DDS_G32R32F        115
#define DDS_A32B32G32R32F  116

#define DDS_FORMAT_R32G32B32A32_FLOAT  2
#define DDS_FORMAT_R32G32B32A32_UINT   3
#define DDS_FORMAT_R16G16B16A16_FLOAT  10
#define DDS_FORMAT_R16G16B16A16_UNORM  11
#define DDS_FORMAT_R16G16B16A16_UINT   12
#define DDS_FORMAT_R32G32_FLOAT        16
#define DDS_FORMAT_R32G32_UINT         17
#define DDS_FORMAT_R10G10B10A2_UNORM   24
#define DDS_FORMAT_R11G11B10_FLOAT     26
#define DDS_FORMAT_R8G8B8A8_UNORM      28
#define DDS_FORMAT_R8G8B8A8_UNORM_SRGB 29
#define DDS_FORMAT_R16G16_FLOAT        34
#define DDS_FORMAT_R16G16_UNORM        35
#define DDS_FORMAT_R32_FLOAT           41
#define DDS_FORMAT_R32_UINT            42
#define DDS_FORMAT_R8G8_UNORM          49
#define DDS_FORMAT_R16_FLOAT           54
#define DDS_FORMAT_R16_UNORM           56
#define DDS_FORMAT_R8_UNORM            61
#define DDS_FORMAT_R1_UNORM            66
#define DDS_FORMAT_BC1_UNORM           71
#define DDS_FORMAT_BC1_UNORM_SRGB      72
#define DDS_FORMAT_BC2_UNORM           74
#define DDS_FORMAT_BC2_UNORM_SRGB      75
#define DDS_FORMAT_BC3_UNORM           77
#define DDS_FORMAT_BC3_UNORM_SRGB      78
#define DDS_FORMAT_BC4_UNORM           80
#define DDS_FORMAT_BC5_UNORM           83
#define DDS_FORMAT_B5G6R5_UNORM        85
#define DDS_FORMAT_B5G5R5A1_UNORM      86
#define DDS_FORMAT_B8G8R8A8_UNORM      87
#define DDS_FORMAT_B8G8R8A8_UNORM_SRGB 91
#define DDS_FORMAT_BC6H_SF16           96
#define DDS_FORMAT_BC7_UNORM           98
#define DDS_FORMAT_BC7_UNORM_SRGB      99
#define DDS_FORMAT_B4G4R4A4_UNORM      115

#define DDS_DX10_DIMENSION_TEXTURE2D 3
#define DDS_DX10_DIMENSION_TEXTURE3D 4
#define DDS_DX10_MISC_TEXTURECUBE    4

#define DDSD_CAPS                  0x00000001
#define DDSD_HEIGHT                0x00000002
#define DDSD_WIDTH                 0x00000004
#define DDSD_PITCH                 0x00000008
#define DDSD_PIXELFORMAT           0x00001000
#define DDSD_MIPMAPCOUNT           0x00020000
#define DDSD_LINEARSIZE            0x00080000
#define DDSD_DEPTH                 0x00800000

#define DDPF_ALPHAPIXELS           0x00000001
#define DDPF_ALPHA                 0x00000002
#define DDPF_FOURCC                0x00000004
#define DDPF_INDEXED               0x00000020
#define DDPF_RGB                   0x00000040
#define DDPF_YUV                   0x00000200
#define DDPF_LUMINANCE             0x00020000
#define DDPF_BUMPDUDV              0x00080000

#define DDSCAPS_COMPLEX            0x00000008
#define DDSCAPS_TEXTURE            0x00001000
#define DDSCAPS_MIPMAP             0x00400000

#define DDSCAPS2_VOLUME            0x00200000
#define DDSCAPS2_CUBEMAP           0x00000200
#define DDSCAPS2_CUBEMAP_POSITIVEX 0x00000400
#define DDSCAPS2_CUBEMAP_NEGATIVEX 0x00000800
#define DDSCAPS2_CUBEMAP_POSITIVEY 0x00001000
#define DDSCAPS2_CUBEMAP_NEGATIVEY 0x00002000
#define DDSCAPS2_CUBEMAP_POSITIVEZ 0x00004000
#define DDSCAPS2_CUBEMAP_NEGATIVEZ 0x00008000

#define DSCAPS2_CUBEMAP_ALLSIDES (0      \
			| DDSCAPS2_CUBEMAP_POSITIVEX \
			| DDSCAPS2_CUBEMAP_NEGATIVEX \
			| DDSCAPS2_CUBEMAP_POSITIVEY \
			| DDSCAPS2_CUBEMAP_NEGATIVEY \
			| DDSCAPS2_CUBEMAP_POSITIVEZ \
			| DDSCAPS2_CUBEMAP_NEGATIVEZ \
			)

union DDSHeader
{
	struct
	{
		uint32_t Magic;
		uint32_t Size;
		uint32_t Flags;
		uint32_t Height;
		uint32_t Width;
		uint32_t PitchOrLinearSize;
		uint32_t Depth;
		uint32_t MipMapCount;
		uint32_t Reserved1[11];

		struct
		{
			uint32_t Size;
			uint32_t Flags;
			uint32_t FourCC;
			uint32_t RGBBitCount;
			uint32_t RBitMask;
			uint32_t GBitMask;
			uint32_t BBitMask;
			uint32_t AlphaBitMask;
		} PixelFormat;

		struct
		{
			uint32_t Caps1;
			uint32_t Caps2;
			uint32_t DDSX;
			uint32_t Reserved;
		} Caps;

		uint32_t Reserved2;
		
		struct
		{
			uint32_t DXGIFormat;
			uint32_t Dims;
			uint32_t MiscFlags;
			uint32_t ArraySize;
			uint32_t MiscFlags2;
		} DX10Info;

	} Header;

	uint8_t data[128];
};

struct TranslateDdsFormat
{
	uint32_t m_format;
	TextureFormat::Enum m_textureFormat;
	bool m_srgb;
};

static const TranslateDdsFormat s_translateDdsFourccFormat[] =
{
	{ DDS_DXT1,                  TextureFormat::BC1,     false },
	{ DDS_DXT2,                  TextureFormat::BC2,     false },
	{ DDS_DXT3,                  TextureFormat::BC2,     false },
	{ DDS_DXT4,                  TextureFormat::BC3,     false },
	{ DDS_DXT5,                  TextureFormat::BC3,     false },
	{ DDS_ATI1,                  TextureFormat::BC4,     false },
	{ DDS_BC4U,                  TextureFormat::BC4,     false },
	{ DDS_ATI2,                  TextureFormat::BC5,     false },
	{ DDS_BC5U,                  TextureFormat::BC5,     false },
	{ DDS_A16B16G16R16,          TextureFormat::RGBA16,  false },
	{ DDS_A16B16G16R16F,         TextureFormat::RGBA16F, false },
	{ DDPF_RGB | DDPF_ALPHAPIXELS, TextureFormat::BGRA8,   false },
	{ DDPF_INDEXED,              TextureFormat::R8,      false },
	{ DDPF_LUMINANCE,            TextureFormat::R8,      false },
	{ DDPF_ALPHA,                TextureFormat::R8,      false },
	{ DDS_R16F,                  TextureFormat::R16F,    false },
	{ DDS_R32F,                  TextureFormat::R32F,    false },
	{ DDS_A8L8,                  TextureFormat::RG8,     false },
	{ DDS_G16R16,                TextureFormat::RG16,    false },
	{ DDS_G16R16F,               TextureFormat::RG16F,   false },
	{ DDS_G32R32F,               TextureFormat::RG32F,   false },
	{ DDS_A8R8G8B8,              TextureFormat::BGRA8,   false },
	{ DDS_A16B16G16R16,          TextureFormat::RGBA16,  false },
	{ DDS_A16B16G16R16F,         TextureFormat::RGBA16F, false },
	{ DDS_A32B32G32R32F,         TextureFormat::RGBA32F, false },
	{ DDS_R5G6B5,                TextureFormat::R5G6B5,  false },
	{ DDS_A4R4G4B4,              TextureFormat::RGBA4,   false },
	{ DDS_A1R5G5B5,              TextureFormat::RGB5A1,  false },
	{ DDS_A2B10G10R10,           TextureFormat::RGB10A2, false },
};

static const TranslateDdsFormat s_translateDxgiFormat[] =
{
	{ DDS_FORMAT_BC1_UNORM,           TextureFormat::BC1,        false },
	{ DDS_FORMAT_BC1_UNORM_SRGB,      TextureFormat::BC1,        true },
	{ DDS_FORMAT_BC2_UNORM,           TextureFormat::BC2,        false },
	{ DDS_FORMAT_BC2_UNORM_SRGB,      TextureFormat::BC2,        true },
	{ DDS_FORMAT_BC3_UNORM,           TextureFormat::BC3,        false },
	{ DDS_FORMAT_BC3_UNORM_SRGB,      TextureFormat::BC3,        true },
	{ DDS_FORMAT_BC4_UNORM,           TextureFormat::BC4,        false },
	{ DDS_FORMAT_BC5_UNORM,           TextureFormat::BC5,        false },
	{ DDS_FORMAT_BC6H_SF16,           TextureFormat::BC6H,       false },
	{ DDS_FORMAT_BC7_UNORM,           TextureFormat::BC7,        false },
	{ DDS_FORMAT_BC7_UNORM_SRGB,      TextureFormat::BC7,        true },

	{ DDS_FORMAT_R1_UNORM,            TextureFormat::R1,         false },
	{ DDS_FORMAT_R8_UNORM,            TextureFormat::R8,         false },
	{ DDS_FORMAT_R16_UNORM,           TextureFormat::R16,        false },
	{ DDS_FORMAT_R16_FLOAT,           TextureFormat::R16F,       false },
	{ DDS_FORMAT_R32_UINT,            TextureFormat::R32U,       false },
	{ DDS_FORMAT_R32_FLOAT,           TextureFormat::R32F,       false },
	{ DDS_FORMAT_R8G8_UNORM,          TextureFormat::RG8,        false },
	{ DDS_FORMAT_R16G16_UNORM,        TextureFormat::RG16,       false },
	{ DDS_FORMAT_R16G16_FLOAT,        TextureFormat::RG16F,      false },
	{ DDS_FORMAT_R32G32_UINT,         TextureFormat::RG32U,      false },
	{ DDS_FORMAT_R32G32_FLOAT,        TextureFormat::RG32F,      false },
	{ DDS_FORMAT_B8G8R8A8_UNORM,      TextureFormat::BGRA8,      false },
	{ DDS_FORMAT_B8G8R8A8_UNORM_SRGB, TextureFormat::BGRA8,      true },
	{ DDS_FORMAT_R8G8B8A8_UNORM,      TextureFormat::RGBA8,      false },
	{ DDS_FORMAT_R8G8B8A8_UNORM_SRGB, TextureFormat::RGBA8,      true },
	{ DDS_FORMAT_R16G16B16A16_UNORM,  TextureFormat::RGBA16,     false },
	{ DDS_FORMAT_R16G16B16A16_FLOAT,  TextureFormat::RGBA16F,    false },
	{ DDS_FORMAT_R32G32B32A32_UINT,   TextureFormat::RGBA32U,    false },
	{ DDS_FORMAT_R32G32B32A32_FLOAT,  TextureFormat::RGBA32F,    false },
	{ DDS_FORMAT_B5G6R5_UNORM,        TextureFormat::R5G6B5,     false },
	{ DDS_FORMAT_B4G4R4A4_UNORM,      TextureFormat::RGBA4,      false },
	{ DDS_FORMAT_B5G5R5A1_UNORM,      TextureFormat::RGB5A1,     false },
	{ DDS_FORMAT_R10G10B10A2_UNORM,   TextureFormat::RGB10A2,    false },
	{ DDS_FORMAT_R11G11B10_FLOAT,     TextureFormat::RG11B10F,   false },
};

struct TranslateDdsPixelFormat
{
	uint32_t m_bitCount;
	uint32_t m_flags;
	uint32_t m_bitmask[4];
	TextureFormat::Enum m_textureFormat;
};

static const TranslateDdsPixelFormat s_translateDdsPixelFormat[] =
{
	{ 8, DDPF_LUMINANCE,{ 0x000000ff, 0x00000000, 0x00000000, 0x00000000 }, TextureFormat::R8 },
	{ 16, DDPF_BUMPDUDV,{ 0x000000ff, 0x0000ff00, 0x00000000, 0x00000000 }, TextureFormat::RG8S },
	{ 16, DDPF_RGB,{ 0x0000ffff, 0x00000000, 0x00000000, 0x00000000 }, TextureFormat::R16U },
	{ 16, DDPF_RGB | DDPF_ALPHAPIXELS,{ 0x00000f00, 0x000000f0, 0x0000000f, 0x0000f000 }, TextureFormat::RGBA4 },
	{ 16, DDPF_RGB,{ 0x0000f800, 0x000007e0, 0x0000001f, 0x00000000 }, TextureFormat::R5G6B5 },
	{ 16, DDPF_RGB,{ 0x00007c00, 0x000003e0, 0x0000001f, 0x00008000 }, TextureFormat::RGB5A1 },
	{ 24, DDPF_RGB,{ 0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000 }, TextureFormat::RGB8 },
	{ 32, DDPF_RGB,{ 0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000 }, TextureFormat::BGRA8 },
	{ 32, DDPF_RGB | DDPF_ALPHAPIXELS,{ 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000 }, TextureFormat::RGBA8 },
	{ 32, DDPF_BUMPDUDV,{ 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000 }, TextureFormat::RGBA8S },
	{ 32, DDPF_RGB,{ 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000 }, TextureFormat::BGRA8 },
	{ 32, DDPF_RGB | DDPF_ALPHAPIXELS,{ 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000 }, TextureFormat::BGRA8 }, // D3DFMT_A8R8G8B8
	{ 32, DDPF_RGB | DDPF_ALPHAPIXELS,{ 0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000 }, TextureFormat::BGRA8 }, // D3DFMT_X8R8G8B8
	{ 32, DDPF_RGB | DDPF_ALPHAPIXELS,{ 0x000003ff, 0x000ffc00, 0x3ff00000, 0xc0000000 }, TextureFormat::RGB10A2 },
	{ 32, DDPF_RGB,{ 0x0000ffff, 0xffff0000, 0x00000000, 0x00000000 }, TextureFormat::RG16 },
	{ 32, DDPF_BUMPDUDV,{ 0x0000ffff, 0xffff0000, 0x00000000, 0x00000000 }, TextureFormat::RG16S },
	{ 32, DDPF_RGB,{ 0xffffffff, 0x00000000, 0x00000000, 0x00000000 }, TextureFormat::R32U },
};



//--------------------------------------------------------------------
bool Image2::Init(const std::string &filename)
{
	FileReader data(filename);
	if ( !data.isLoaded ) return false;

	const int size = static_cast<int>(data.data.size());

	uint32_t magic;
	data.Read(magic);
	data.Seek(0);
	
	// Load DDS
	if ( magic == DDS_MAGIC )
	{
		if ( size < sizeof(DDSHeader) )
		{
			LogError() << "DDS: Texture file length is less than DDS header length.";
			return false;
		}

		DDSHeader header;		
		data.Read(header);
		
		if ( header.Header.Size != DDS_HEADER_SIZE )
		{
			LogError() << "DDS: header not size DDS";
			return false;
		}
		
		if ( (header.Header.Flags & (DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT)) != (DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT) )
		{
			LogError() << "DDS: Invalid flags.";
			return false;
		}

		uint32_t height = header.Header.Height;
		uint32_t width = header.Header.Width;
		uint32_t pitch = header.Header.PitchOrLinearSize;
		uint32_t depth = header.Header.Depth;
		uint32_t mips = header.Header.MipMapCount;

		// DDPIXELFORMAT
		uint32_t pixelFormatSize = header.Header.PixelFormat.Size;
		uint32_t pixelFlags = header.Header.PixelFormat.Flags;		
		uint32_t fourcc = header.Header.PixelFormat.FourCC;
		uint32_t bitCount = header.Header.PixelFormat.RGBBitCount;
		uint32_t bitmask[4];
		bitmask[0] = header.Header.PixelFormat.RBitMask;
		bitmask[1] = header.Header.PixelFormat.GBitMask;
		bitmask[2] = header.Header.PixelFormat.BBitMask;
		bitmask[3] = header.Header.PixelFormat.AlphaBitMask;

		// DDCAPS2
		uint32_t caps[4];
		caps[0] = header.Header.Caps.Caps1;
		caps[1] = header.Header.Caps.Caps2;
		caps[2] = header.Header.Caps.DDSX;
		caps[3] = header.Header.Caps.Reserved;

		uint32_t dxgiFormat = 0;
		uint32_t arraySize = 1;
		if ( DDPF_FOURCC == pixelFlags && DDS_DX10 == fourcc )
		{
			dxgiFormat = header.Header.DX10Info.DXGIFormat;
			//uint32_t dims = header.Header.DX10Info.Dims;
			//uint32_t miscFlags = header.Header.DX10Info.MiscFlags;
			arraySize = header.Header.DX10Info.ArraySize;
			//uint32_t miscFlags2 = header.Header.DX10Info.MiscFlags2;
		}

		if ( (caps[0] & DDSCAPS_TEXTURE) == 0 )
		{
			LogError() << "DDS: Unsupported caps.";
			return false;
		}

		bool cubeMap = 0 != (caps[1] & DDSCAPS2_CUBEMAP);
		if ( cubeMap )
		{
			if ( (caps[1] & DSCAPS2_CUBEMAP_ALLSIDES) != DSCAPS2_CUBEMAP_ALLSIDES )
			{
				// partial cube map is not supported.
				LogError() << "DDS: Incomplete cubemap.";
				return false;
			}
		}

		TextureFormat::Enum format = TextureFormat::Unknown;
		bool hasAlpha = pixelFlags & DDPF_ALPHAPIXELS;
		bool srgb = false;

		if ( dxgiFormat == 0 )
		{
			if ( DDPF_FOURCC == (pixelFlags & DDPF_FOURCC) )
			{
				for ( uint32_t ii = 0; ii < COUNTOF(s_translateDdsFourccFormat); ++ii )
				{
					if ( s_translateDdsFourccFormat[ii].m_format == fourcc )
					{
						format = s_translateDdsFourccFormat[ii].m_textureFormat;
						break;
					}
				}
			}
			else
			{
				for ( uint32_t ii = 0; ii < COUNTOF(s_translateDdsPixelFormat); ++ii )
				{
					const TranslateDdsPixelFormat &pf = s_translateDdsPixelFormat[ii];
					if ( pf.m_bitCount == bitCount
						&& pf.m_flags == pixelFlags
						&& pf.m_bitmask[0] == bitmask[0]
						&& pf.m_bitmask[1] == bitmask[1]
						&& pf.m_bitmask[2] == bitmask[2]
						&& pf.m_bitmask[3] == bitmask[3] )
					{
						format = pf.m_textureFormat;
						break;
					}
				}
			}
		}
		else
		{
			for ( uint32_t ii = 0; ii < COUNTOF(s_translateDxgiFormat); ++ii )
			{
				if ( s_translateDxgiFormat[ii].m_format == dxgiFormat )
				{
					format = s_translateDxgiFormat[ii].m_textureFormat;
					srgb = s_translateDxgiFormat[ii].m_srgb;
					break;
				}
			}
		}

		if ( TextureFormat::Unknown == format )
		{
			LogError() << "DDS: Unknown texture format.";
			return false;
		}

		const uint8_t mipMapCount = uint8_t( ((caps[0] & DDSCAPS_MIPMAP) && (header.Header.Flags & DDSD_MIPMAPCOUNT)) ? mips : 1 );

		const size_t fileOffset = sizeof(header);

		TODO("тут");

		// 2 part
		//===================================================
		imageAlloc(format, 
			uint16_t(width), 
			uint16_t(height), 
			uint16_t(depth), 
			uint16_t(arraySize), 			
			cubeMap, 
			1 < mipMapCount);

		m_offset = (uint32_t)data.Seek(0, Whence::Current);
		m_hasAlpha = hasAlpha;
		m_srgb = srgb;

		const uint16_t numSides = m_numLayers * (m_cubeMap ? 6 : 1);

		for ( uint16_t side = 0; side < numSides; ++side )
		{
			for ( uint8_t lod = 0, num = mipMapCount; lod < num; ++lod )
			{
				ImageMip dstMip;
				if ( imageGetRawData(side, lod, m_size, dstMip) )
				{
					ImageMip mip;
					if ( imageGetRawData(side, lod, size, mip) )
					{
						uint8_t *dstData = const_cast<uint8_t*>(dstMip.m_data);
						memcpy(dstData, mip.m_data, mip.m_size);
					}
				}
			}
		}
	}
	// Load STB
	else 
	{
		const int isHDR = stbi_is_hdr_from_memory(data.data.data(), size);

		void *tempData;
		int width = 0;
		int height = 0;
		int comp = 0;

		if ( isHDR )
			tempData = stbi_loadf_from_memory(data.data.data(), size, &width, &height, &comp, 4);
		else
			tempData = stbi_load_from_memory(data.data.data(), size, &width, &height, &comp, 0);

		if ( !tempData )
		{
			LogError() << "Failed to open texture file " << filename << ", reason: " << stbi_failure_reason();
			return false;
		}

		TextureFormat::Enum format = TextureFormat::RGBA8;
		if ( isHDR )
		{
			format = TextureFormat::RGBA32F;
		}
		else
		{
			switch ( comp )
			{
			case 1: format = TextureFormat::R8; break;
			case 2: format = TextureFormat::RG8; break;
			case 3: format = TextureFormat::RGB8; break;
			default: break;
			}
		}

		imageAlloc(format, uint16_t(width), uint16_t(height), 0, 1, false, false, (const uint8_t*)tempData);
		stbi_image_free(tempData);
	}	

	return true;
}
//--------------------------------------------------------------------
void Image2::imageAlloc(TextureFormat::Enum format, uint16_t width, uint16_t height, uint16_t depth, uint16_t numLayers, bool cubeMap, bool hasMips, const uint8_t *data)
{
	const ImageBlockInfo &blockInfo = getBlockInfo(format);
	const uint16_t blockWidth = blockInfo.blockWidth;
	const uint16_t blockHeight = blockInfo.blockHeight;
	const uint16_t minBlockX = blockInfo.minBlockX;
	const uint16_t minBlockY = blockInfo.minBlockY;

	width = std::max(blockWidth * minBlockX, ((width + blockWidth - 1) / blockWidth) * blockWidth);
	height = std::max(blockHeight * minBlockY, ((height + blockHeight - 1) / blockHeight) * blockHeight);
	depth = std::max(1ui16, depth);
	numLayers = std::max(1ui16, numLayers);

	const uint8_t numMips = hasMips ? imageGetNumMips(format, width, height, depth) : 1;
	const uint32_t size = imageGetSize(/*NULL,*/ width, height, depth, cubeMap, hasMips, numLayers, format);

	if ( data ) m_data.assign(data, data + size);
	m_format = format;
	m_orientation = Orientation::R0;
	m_size = size;
	m_offset = 0;
	m_width = width;
	m_height = height;
	m_depth = depth;
	m_numLayers = numLayers;
	m_numMips = numMips;
	m_hasAlpha = false;
	m_cubeMap = cubeMap;
	m_ktx = false;
	m_ktxLE = false;
	m_srgb = false;
}
//--------------------------------------------------------------------
bool Image2::imageGetRawData(uint16_t _side, uint8_t _lod, uint32_t _size, ImageMip& _mip)
{
	uint32_t offset = m_offset;
	TextureFormat::Enum format = TextureFormat::Enum(m_format);
	bool hasAlpha = m_hasAlpha;

	const ImageBlockInfo &blockInfo = s_imageBlockInfo[format];
	const uint8_t  bpp = blockInfo.bitsPerPixel;
	const uint32_t blockSize = blockInfo.blockSize;
	const uint32_t blockWidth = blockInfo.blockWidth;
	const uint32_t blockHeight = blockInfo.blockHeight;
	const uint32_t minBlockX = blockInfo.minBlockX;
	const uint32_t minBlockY = blockInfo.minBlockY;
	
	const uint8_t *data = m_data.data();
	const uint16_t numSides = m_numLayers * (m_cubeMap ? 6 : 1);

	if ( m_ktx )
	{
		uint32_t width = m_width;
		uint32_t height = m_height;
		uint32_t depth = m_depth;

		for ( uint8_t lod = 0, num = m_numMips; lod < num; ++lod )
		{
			width = std::max(blockWidth  * minBlockX, ((width + blockWidth - 1) / blockWidth)*blockWidth);
			height = std::max(blockHeight * minBlockY, ((height + blockHeight - 1) / blockHeight)*blockHeight);
			depth = std::max(1u, depth);

			const uint32_t mipSize = width*height*depth*bpp / 8;

			const uint32_t size = mipSize*numSides;
			uint32_t imageSize = toHostEndian(*(const uint32_t*)&data[offset], m_ktxLE);
			Assert(size == imageSize && "KTX: Image2 size mismatch");

			offset += sizeof(uint32_t);

			for ( uint16_t side = 0; side < numSides; ++side )
			{
				if ( side == _side
					&&  lod == _lod )
				{
					_mip.m_width = width;
					_mip.m_height = height;
					_mip.m_depth = depth;
					_mip.m_blockSize = blockSize;
					_mip.m_size = mipSize;
					_mip.m_data = &data[offset];
					_mip.m_bpp = bpp;
					_mip.m_format = format;
					_mip.m_hasAlpha = hasAlpha;
					return true;
				}

				offset += mipSize;

				Assert(offset <= _size && "Reading past size of data buffer!");
			}

			width >>= 1;
			height >>= 1;
			depth >>= 1;
		}
	}
	else
	{
		for ( uint16_t side = 0; side < numSides; ++side )
		{
			uint32_t width = m_width;
			uint32_t height = m_height;
			uint32_t depth = m_depth;

			for ( uint8_t lod = 0, num = m_numMips; lod < num; ++lod )
			{
				width = std::max(blockWidth  * minBlockX, ((width + blockWidth - 1) / blockWidth)*blockWidth);
				height = std::max(blockHeight * minBlockY, ((height + blockHeight - 1) / blockHeight)*blockHeight);
				depth = std::max(1u, depth);

				uint32_t size = width*height*depth*bpp / 8;

				if ( side == _side
					&&  lod == _lod )
				{
					_mip.m_width = width;
					_mip.m_height = height;
					_mip.m_depth = depth;
					_mip.m_blockSize = blockSize;
					_mip.m_size = size;
					_mip.m_data = &data[offset];
					_mip.m_bpp = bpp;
					_mip.m_format = format;
					_mip.m_hasAlpha = hasAlpha;
					return true;
				}

				offset += size;

				Assert(offset <= _size && "Reading past size of data buffer!");

				width >>= 1;
				height >>= 1;
				depth >>= 1;
			}
		}
	}

	return false;
}