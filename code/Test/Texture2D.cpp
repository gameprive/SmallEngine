// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "Texture2D.h"
#include <stb_image.h>
#include "DDSLoader.h"
#include "HasExtension.h"
#include "Core/IO/FileSystem.h"

bool HasStbExtension(const std::string& path); // Defined in TextureCommon.cpp
void TexReload(const std::string& path); // Defined in TextureCommon.cpp
float GetFloatAnisotropy(Anisotropy anisotropy);
void Tokenize(const std::string& str,
	std::vector< std::string >& tokens,
	const std::string& delimiters = " "); // Defined in TextureCommon.cpp

namespace Texture2DGlobal
{
	nTexture2D defaultTexture;

	std::map< std::string, nTexture2D > hashToCachedTexture;
#if _DEBUG
	std::map< std::string, std::size_t > pathToCachedTextureSizeInBytes;

	void PrintMemoryUsage()
	{
		std::size_t total = 0;

		for ( const auto& path : pathToCachedTextureSizeInBytes )
		{
			Log() << path.first << ": " << path.second << " B";
			total += path.second;
		}
		
		Log() << "Total texture usage: " << total / 1024 << " KiB";
	}
#endif
}

nTexture2D* nTexture2D::GetDefaultTexture()
{
	if ( Texture2DGlobal::defaultTexture.GetWidth() == 0 )
	{
		Texture2DGlobal::defaultTexture.width = 32;
		Texture2DGlobal::defaultTexture.height = 32;

		glGenTextures(1, &Texture2DGlobal::defaultTexture.handle);

		glBindTexture(GL_TEXTURE_2D, Texture2DGlobal::defaultTexture.handle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		if ( HasExtension("GL_KHR_debug") )
		{
			glObjectLabel(GL_TEXTURE, Texture2DGlobal::defaultTexture.handle, -1, "default texture 2d");
		}

		int data[32 * 32 * 4] = { 0xFFFFFF };
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Texture2DGlobal::defaultTexture.width, Texture2DGlobal::defaultTexture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		ErrorCheck("Load nTexture2D");
	}

	return &Texture2DGlobal::defaultTexture;
}

void nTexture2D::LoadFromData(const void* imageData, int aWidth, int aHeight, int channels, const char* debugName)
{
	width = aWidth;
	height = aHeight;
	wrap = TextureWrap::Repeat;
	filter = TextureFilter::Linear;

	glGenTextures(1, &handle);

	glBindTexture(GL_TEXTURE_2D, handle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glPixelStorei(GL_UNPACK_ALIGNMENT, channels == 4 ? 4 : 1);

	if ( HasExtension("GL_KHR_debug") )
	{
		glObjectLabel(GL_TEXTURE, handle, -1, debugName);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, channels == 4 ? GL_RGBA8 : GL_RGB8, width, height, 0, channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, imageData);
}

void nTexture2D::Load(const std::string &path, const std::vector<uint8_t>& fileContents, TextureWrap aWrap, TextureFilter aFilter, Mipmaps aMipmaps, ColorSpace aColorSpace, Anisotropy aAnisotropy)
{
	filter = aFilter;
	wrap = aWrap;
	mipmaps = aMipmaps;
	anisotropy = aAnisotropy;
	colorSpace = aColorSpace;
	this->path = path;

	//if ( !fileContents.isLoaded )
	//{
	//	*this = Texture2DGlobal::defaultTexture;
	//	return;
	//}

	const std::string cacheHash = GetCacheHash(path, aWrap, aFilter, aMipmaps, aColorSpace, aAnisotropy);
	const bool isCached = Texture2DGlobal::hashToCachedTexture.find(cacheHash) != Texture2DGlobal::hashToCachedTexture.end();

	if ( isCached && handle == 0 )
	{
		*this = Texture2DGlobal::hashToCachedTexture[cacheHash];
		return;
	}

	// First load.
	if ( handle == 0 )
	{
		glGenTextures(1, &handle);
		//fileWatcher.AddFile(path, TexReload);
	}

	GLenum magFilter = GL_NEAREST;
	GLenum minFilter = GL_NEAREST;

	if ( filter == TextureFilter::Linear )
	{
		minFilter = GL_LINEAR;
		magFilter = GL_LINEAR;

		if ( aMipmaps == Mipmaps::Generate )
		{
			minFilter = GL_LINEAR_MIPMAP_LINEAR;
		}
	}

	glBindTexture(GL_TEXTURE_2D, handle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (wrap == TextureWrap::Repeat) ? GL_REPEAT : GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (wrap == TextureWrap::Repeat) ? GL_REPEAT : GL_CLAMP_TO_EDGE);

	if ( HasExtension("GL_KHR_debug") )
	{
		glObjectLabel(GL_TEXTURE, handle, -1, path.length() < 256 ? path.c_str() : "long texture2d name");
	}

	if ( HasExtension("GL_EXT_texture_filter_anisotropic") && anisotropy != Anisotropy::k1 )
	{
		glTexParameterf(GL_TEXTURE_2D, 0x84FE/*GL_TEXTURE_MAX_ANISOTROPY_EXT*/, GetFloatAnisotropy(anisotropy));
	}

	const bool isDDS = path.find(".dds") != std::string::npos || path.find(".DDS") != std::string::npos;

	if ( HasStbExtension(path) )
	{
		LoadSTB(path, fileContents);
	}
	else if ( isDDS && HasExtension("GL_EXT_texture_compression_s3tc") )
	{
		LoadDDS(path.c_str());
	}
	else
	{
		Log(LevelLog::Error) << "Unhandled texture extension in file " <<  path;
		*this = Texture2DGlobal::defaultTexture;
		return;
	}

	if ( mipmaps == Mipmaps::Generate )
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	Texture2DGlobal::hashToCachedTexture[cacheHash] = *this;
#if _DEBUG
	Texture2DGlobal::pathToCachedTextureSizeInBytes[path] = static_cast<size_t>(width * height * 4 * (mipmaps == Mipmaps::Generate ? 1.0f : 1.33333f));
#endif
	ErrorCheck("Load nTexture2D");
}

void nTexture2D::LoadDDS(const char* aPath)
{
	DDSLoader::Output unusedOutput;

	std::vector<uint8_t> data;
	FileSystem::ReadFile(aPath, data);

	const DDSLoader::LoadResult loadResult = DDSLoader::Load(data, 0, width, height, opaque, unusedOutput);

	if ( loadResult != DDSLoader::LoadResult::Success )
	{
		Log(LevelLog::Error) << "DDS Loader could not load " << aPath;
	}
}

void nTexture2D::LoadSTB(const std::string &path, const std::vector<uint8_t>& fileContents)
{
	int components;
	unsigned char* data = stbi_load_from_memory(fileContents.data(), static_cast<int>(fileContents.size()), &width, &height, &components, 4);

	if ( data == nullptr )
	{
		const std::string reason(stbi_failure_reason());
		Log(LevelLog::Error) << path << "failed to load.stb_image's reason: " << reason;
		return;
	}

	opaque = (components == 3 || components == 1);

	// FIXME: Disabled because immutable textures cannot be reloaded. Enable for shipping builds for performance.
	/*if (GfxDevice::HasExtension( "GL_ARB_texture_storage" ))
	{
	glTexStorage2D( GL_TEXTURE_2D, 1, colorSpace == ColorSpace::RGB ? GL_RGBA8 : GL_SRGB8_ALPHA8, width, height );
	glTexSubImage2D( GL_TEXTURE_2D,
	0,
	0, 0,
	width, height,
	GL_RGBA,
	GL_UNSIGNED_BYTE,
	data );
	}
	else*/
	{
		glTexImage2D(GL_TEXTURE_2D, 0, colorSpace == ColorSpace::RGB ? GL_RGBA8 : GL_SRGB8_ALPHA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}

	ErrorCheck("Load nTexture2D");
	stbi_image_free(data);
}
