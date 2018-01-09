// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "DDSLoader.h"

unsigned MyMax(unsigned a, unsigned b)
{
	return (a > b ? a : b);
}

#ifndef GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83F1
#endif

#ifndef GL_COMPRESSED_RGBA_S3TC_DXT3_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2
#endif

#ifndef GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3
#endif

#ifndef GL_COMPRESSED_SRGB_S3TC_DXT1_EXT
#define GL_COMPRESSED_SRGB_S3TC_DXT1_EXT 0x8C4C
#endif

#ifndef COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT
#define COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT 0x8C4D
#endif

#ifndef COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT
#define COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT 0x8C4E
#endif

#ifndef COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT
#define COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT 0x8C4F
#endif

#ifndef COMPRESSED_SRGB_EXT
#define COMPRESSED_SRGB_EXT 0x8C48
#endif

DDSInfo loadInfoDXT1 = { true, false, false, 4, 8, GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, GL_RGBA, GL_UNSIGNED_BYTE };

DDSInfo loadInfoDXT3 = { true, false, false, 4, 16, GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, GL_RGBA, GL_UNSIGNED_BYTE };

DDSInfo loadInfoDXT5 = { true, false, false, 4, 16, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, GL_RGBA, GL_UNSIGNED_BYTE };

DDSInfo loadInfoBGRA8 = { false, false, false, 1, 4, GL_RGBA8, GL_BGRA, GL_UNSIGNED_BYTE };

DDSInfo loadInfoBGR8 = { false, false, false, 1, 3, GL_RGB8, GL_BGR, GL_UNSIGNED_BYTE };

DDSInfo loadInfoBGR5A1 = { false, true, false, 1, 2, GL_RGB5_A1, GL_BGRA, GL_UNSIGNED_SHORT_1_5_5_5_REV };

DDSInfo loadInfoBGR565 = { false, true, false, 1, 2, GL_RGB5, GL_RGB, GL_UNSIGNED_SHORT_5_6_5 };

DDSInfo loadInfoIndex8 = { false, false, true, 1, 1, GL_RGB8, GL_BGRA, GL_UNSIGNED_BYTE };

DDSLoader::LoadResult DDSLoader::Load(const std::vector<uint8_t>& fileContents, int cubeMapFace, int& outWidth, int& outHeight, bool& outOpaque, Output& output)
{
	assert(cubeMapFace >= 0 && cubeMapFace < 7);

	DDSHeader header;
	int mipMapCount = 0;
	
	if ( fileContents.size() < sizeof(header) )
	{
		Log(LevelLog::Error) << "DDS loader error: Texture file length is less than DDS header length.";
		return LoadResult::FileNotFound;
	}

	std::memcpy(&header, fileContents.data(), sizeof(header));

	assert(header.sHeader.dwMagic == DDS_MAGIC);
	assert(header.sHeader.dwSize == 124);

	if ( !(header.sHeader.dwFlags & DDSD_PIXELFORMAT) || !(header.sHeader.dwFlags & DDSD_CAPS) )
	{
		Log(LevelLog::Error) << "DDS loader error: Texture doesn't contain pixelformat or caps.";
		outWidth = 32;
		outHeight = 32;
		outOpaque = true;
		return LoadResult::UnknownPixelFormat;
	}

	const uint32_t xSize = header.sHeader.dwWidth;
	const uint32_t ySize = header.sHeader.dwHeight;
	assert(!(xSize & (xSize - 1)));
	assert(!(ySize & (ySize - 1)));

	outWidth = xSize;
	outHeight = ySize;
	DDSInfo* li = nullptr;

	if ( PF_IS_DXT1(header.sHeader.sPixelFormat) )
	{
		li = &loadInfoDXT1;
		outOpaque = true;
		output.format = DDSLoader::Format::BC1;
	}
	else if ( PF_IS_DXT3(header.sHeader.sPixelFormat) )
	{
		li = &loadInfoDXT3;
		outOpaque = false;
		output.format = DDSLoader::Format::BC2;
	}
	else if ( PF_IS_DXT5(header.sHeader.sPixelFormat) )
	{
		li = &loadInfoDXT5;
		outOpaque = false;
		output.format = DDSLoader::Format::BC3;
	}
	else if ( PF_IS_BGRA8(header.sHeader.sPixelFormat) )
	{
		li = &loadInfoBGRA8;
		outOpaque = false;
		output.format = DDSLoader::Format::Invalid;
	}
	else if ( PF_IS_BGR8(header.sHeader.sPixelFormat) )
	{
		li = &loadInfoBGR8;
		outOpaque = true;
		output.format = DDSLoader::Format::Invalid;
	}
	else if ( PF_IS_BGR5A1(header.sHeader.sPixelFormat) )
	{
		li = &loadInfoBGR5A1;
		outOpaque = false;
		output.format = DDSLoader::Format::Invalid;
	}
	else if ( PF_IS_BGR565(header.sHeader.sPixelFormat) )
	{
		li = &loadInfoBGR565;
		outOpaque = true;
		output.format = DDSLoader::Format::Invalid;
	}
	else if ( PF_IS_INDEX8(header.sHeader.sPixelFormat) )
	{
		li = &loadInfoIndex8;
		outOpaque = true;
		output.format = DDSLoader::Format::Invalid;
	}
	else
	{
		Log(LevelLog::Error) << "DDS loader error: Texture has unknown pixelformat.";
		outWidth = 32;
		outHeight = 32;
		outOpaque = true;
		return LoadResult::UnknownPixelFormat;
	}

	unsigned x = xSize;
	unsigned y = ySize;
	std::size_t size;
	mipMapCount = (header.sHeader.dwFlags & DDSD_MIPMAPCOUNT) ? header.sHeader.dwMipMapCount : 1;

	if ( mipMapCount == 0 )
	{
		glTexParameteri(cubeMapFace > 0 ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	std::size_t fileOffset = sizeof(header);

	if ( li->isCompressed )
	{
		size = MyMax(li->divSize, x) / li->divSize * MyMax(li->divSize, y) / li->divSize * li->blockBytes;
		assert(size == header.sHeader.dwPitchOrLinearSize);
		assert(header.sHeader.dwFlags & DDSD_LINEARSIZE);

		std::vector< unsigned char > data(size);

		if ( data.empty() )
		{
			Log(LevelLog::Error) << "DDS loader error: Texture contents are empty.";
			outWidth = 32;
			outHeight = 32;
			outOpaque = true;
			return LoadResult::FileNotFound;
		}

		output.imageData = fileContents;
		output.dataOffsets.resize(mipMapCount);

		for ( int ix = 0; ix < mipMapCount; ++ix )
		{
			std::memcpy(data.data(), fileContents.data() + fileOffset, size);
			output.dataOffsets[ix] = fileOffset;

			fileOffset += size;
			glCompressedTexImage2D(cubeMapFace > 0 ? GL_TEXTURE_CUBE_MAP_POSITIVE_X + cubeMapFace - 1 : GL_TEXTURE_2D, ix, li->internalFormat, x, y, 0, (GLsizei)size, &data[0]);

			x = (x + 1) >> 1;
			y = (y + 1) >> 1;
			size = MyMax(li->divSize, x) / li->divSize * MyMax(li->divSize, y) / li->divSize * li->blockBytes;
		}
	}
	else if ( li->hasPalette )
	{
		assert(header.sHeader.dwFlags & DDSD_PITCH);
		assert(header.sHeader.sPixelFormat.dwRGBBitCount == 8);
		size = header.sHeader.dwPitchOrLinearSize * ySize;
		assert(size == x * y * li->blockBytes);
		std::vector< unsigned char > data(size);
		unsigned palette[256];

		std::vector< unsigned > unpacked(size * 4);

		std::memcpy(&palette[0], fileContents.data() + fileOffset, 4 * 256);
		fileOffset += 4 * 256;

		for ( int ix = 0; ix < mipMapCount; ++ix )
		{
			std::memcpy(data.data(), fileContents.data() + fileOffset, size);
			fileOffset += size;

			for ( unsigned zz = 0; zz < size; ++zz )
			{
				unpacked[zz] = palette[data[zz]];
			}

			glPixelStorei(GL_UNPACK_ROW_LENGTH, y);
			glTexImage2D(cubeMapFace > 0 ? GL_TEXTURE_CUBE_MAP_POSITIVE_X + cubeMapFace - 1 : GL_TEXTURE_2D, ix, li->internalFormat, x, y, 0, li->externalFormat, li->type, &unpacked[0]);

			x = (x + 1) >> 1;
			y = (y + 1) >> 1;
			size = x * y * li->blockBytes;
		}
	}
	else
	{
		if ( li->swap )
		{
			glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_TRUE);
		}

		size = x * y * li->blockBytes;
		std::vector< unsigned char > data(size);

		for ( int ix = 0; ix < mipMapCount; ++ix )
		{
			std::memcpy(data.data(), fileContents.data() + fileOffset, size);
			fileOffset += size;

			glPixelStorei(GL_UNPACK_ROW_LENGTH, y);
			glTexImage2D(cubeMapFace > 0 ? GL_TEXTURE_CUBE_MAP_POSITIVE_X + cubeMapFace - 1 : GL_TEXTURE_2D, ix, li->internalFormat, x, y, 0, li->externalFormat, li->type, &data[0]);

			x = (x + 1) >> 1;
			y = (y + 1) >> 1;
			size = x * y * li->blockBytes;
		}
		glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
	}

	glTexParameteri(cubeMapFace > 0 ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipMapCount - 1);

	return LoadResult::Success;
}