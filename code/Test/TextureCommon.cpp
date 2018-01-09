// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "Texture2D.h"
#include "Core/IO/FileSystem.h"

bool HasStbExtension(const std::string& path)
{
	// Checks for uncompressed formats in texture's file name.
	static const std::string extensions[] =
	{
		".png", ".PNG", ".jpg", ".JPG", ".tga", ".TGA",
		".bmp", ".BMP", ".gif", ".GIF"
	};

	const bool extensionFound = std::any_of(std::begin(extensions), std::end(extensions),
		[&](const std::string& extension) { return path.find(extension) != std::string::npos; });

	return extensionFound;
}

void Tokenize(const std::string& str,
	std::vector< std::string >& tokens,
	const std::string& delimiters = " ")
{
	// Skip delimiters at beginning.
	std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	// Find first "non-delimiter".
	std::string::size_type pos = str.find_first_of(delimiters, lastPos);

	while ( std::string::npos != pos || std::string::npos != lastPos )
	{
		// Found a token, add it to the vector.
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		// Skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);
		// Find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}
}

float GetFloatAnisotropy(Anisotropy anisotropy)
{
	if ( anisotropy == Anisotropy::k1 )
	{
		return 1;
	}
	if ( anisotropy == Anisotropy::k2 )
	{
		return 2;
	}
	if ( anisotropy == Anisotropy::k4 )
	{
		return 4;
	}
	if ( anisotropy == Anisotropy::k8 )
	{
		return 8;
	}

	//System::Assert(false, "unhandled anisotropy");
	return 1;
}

namespace Texture2DGlobal
{
	extern std::map< std::string, nTexture2D > hashToCachedTexture;
}

std::string GetCacheHash(const std::string& path, TextureWrap wrap, TextureFilter filter, Mipmaps mipmaps, ColorSpace colorSpace, Anisotropy anisotropy)
{
	return path + std::to_string(static_cast<int>(wrap)) + std::to_string(static_cast<int>(filter)) +
		std::to_string(static_cast<int>(mipmaps)) + std::to_string(static_cast<int>(colorSpace)) + std::to_string(static_cast< int >(anisotropy));
}

void TexReload(const std::string& path)
{
	std::vector<uint8_t> data;
	FileSystem::ReadFile(path, data);

	std::string cacheHash = GetCacheHash(path, TextureWrap::Clamp, TextureFilter::Nearest, Mipmaps::None, ColorSpace::SRGB, Anisotropy::k1);

	if ( Texture2DGlobal::hashToCachedTexture.find(cacheHash) != std::end(Texture2DGlobal::hashToCachedTexture) )
	{
		auto& tex = Texture2DGlobal::hashToCachedTexture[cacheHash];
		tex.Load(path, data, tex.GetWrap(), tex.GetFilter(), tex.GetMipmaps(), tex.GetColorSpace(), tex.GetAnisotropy());
	}

	cacheHash = GetCacheHash(path, TextureWrap::Repeat, TextureFilter::Nearest, Mipmaps::Generate, ColorSpace::RGB, Anisotropy::k1);

	if ( Texture2DGlobal::hashToCachedTexture.find(cacheHash) != std::end(Texture2DGlobal::hashToCachedTexture) )
	{
		auto& tex = Texture2DGlobal::hashToCachedTexture[cacheHash];
		tex.Load(path, data, tex.GetWrap(), tex.GetFilter(), tex.GetMipmaps(), tex.GetColorSpace(), tex.GetAnisotropy());
	}

	cacheHash = GetCacheHash(path, TextureWrap::Repeat, TextureFilter::Nearest, Mipmaps::None, ColorSpace::SRGB, Anisotropy::k1);

	if ( Texture2DGlobal::hashToCachedTexture.find(cacheHash) != std::end(Texture2DGlobal::hashToCachedTexture) )
	{
		auto& tex = Texture2DGlobal::hashToCachedTexture[cacheHash];
		tex.Load(path, data, tex.GetWrap(), tex.GetFilter(), tex.GetMipmaps(), tex.GetColorSpace(), tex.GetAnisotropy());
	}

	cacheHash = GetCacheHash(path, TextureWrap::Clamp, TextureFilter::Linear, Mipmaps::None, ColorSpace::SRGB, Anisotropy::k1);

	if ( Texture2DGlobal::hashToCachedTexture.find(cacheHash) != std::end(Texture2DGlobal::hashToCachedTexture) )
	{
		auto& tex = Texture2DGlobal::hashToCachedTexture[cacheHash];
		tex.Load(path, data, tex.GetWrap(), tex.GetFilter(), tex.GetMipmaps(), tex.GetColorSpace(), tex.GetAnisotropy());
	}

	cacheHash = GetCacheHash(path, TextureWrap::Repeat, TextureFilter::Linear, Mipmaps::None, ColorSpace::SRGB, Anisotropy::k1);

	if ( Texture2DGlobal::hashToCachedTexture.find(cacheHash) != std::end(Texture2DGlobal::hashToCachedTexture) )
	{
		auto& tex = Texture2DGlobal::hashToCachedTexture[cacheHash];
		tex.Load(path, data, tex.GetWrap(), tex.GetFilter(), tex.GetMipmaps(), tex.GetColorSpace(), tex.GetAnisotropy());
	}

	cacheHash = GetCacheHash(path, TextureWrap::Clamp, TextureFilter::Linear, Mipmaps::Generate, ColorSpace::SRGB, Anisotropy::k1);

	if ( Texture2DGlobal::hashToCachedTexture.find(cacheHash) != std::end(Texture2DGlobal::hashToCachedTexture) )
	{
		auto& tex = Texture2DGlobal::hashToCachedTexture[cacheHash];
		tex.Load(path, data, tex.GetWrap(), tex.GetFilter(), tex.GetMipmaps(), tex.GetColorSpace(), tex.GetAnisotropy());
	}

	cacheHash = GetCacheHash(path, TextureWrap::Repeat, TextureFilter::Linear, Mipmaps::Generate, ColorSpace::SRGB, Anisotropy::k1);

	if ( Texture2DGlobal::hashToCachedTexture.find(cacheHash) != std::end(Texture2DGlobal::hashToCachedTexture) )
	{
		auto& tex = Texture2DGlobal::hashToCachedTexture[cacheHash];
		tex.Load(path, data, tex.GetWrap(), tex.GetFilter(), tex.GetMipmaps(), tex.GetColorSpace(), tex.GetAnisotropy());
	}

	cacheHash = GetCacheHash(path, TextureWrap::Repeat, TextureFilter::Nearest, Mipmaps::Generate, ColorSpace::SRGB, Anisotropy::k1);

	if ( Texture2DGlobal::hashToCachedTexture.find(cacheHash) != std::end(Texture2DGlobal::hashToCachedTexture) )
	{
		auto& tex = Texture2DGlobal::hashToCachedTexture[cacheHash];
		tex.Load(path, data, tex.GetWrap(), tex.GetFilter(), tex.GetMipmaps(), tex.GetColorSpace(), tex.GetAnisotropy());
	}

	cacheHash = GetCacheHash(path, TextureWrap::Clamp, TextureFilter::Nearest, Mipmaps::Generate, ColorSpace::SRGB, Anisotropy::k1);

	if ( Texture2DGlobal::hashToCachedTexture.find(cacheHash) != std::end(Texture2DGlobal::hashToCachedTexture) )
	{
		auto& tex = Texture2DGlobal::hashToCachedTexture[cacheHash];
		tex.Load(path, data, tex.GetWrap(), tex.GetFilter(), tex.GetMipmaps(), tex.GetColorSpace(), tex.GetAnisotropy());
	}
}

//void nTexture2D::LoadFromAtlas(const std::vector<uint8_t>& atlasTextureData, const std::vector<uint8_t>& atlasMetaData, const char* textureName, TextureWrap aWrap, TextureFilter aFilter, ColorSpace aColorSpace, Anisotropy aAnisotropy)
//{
//	Load(atlasTextureData, aWrap, aFilter, mipmaps, aColorSpace, aAnisotropy);
//
//	const std::string metaStr = std::string(std::begin(atlasMetaData.data), std::end(atlasMetaData.data));
//	std::stringstream metaStream(metaStr);
//
//	if ( atlasMetaData.path.find(".xml") == std::string::npos && atlasMetaData.path.find(".XML") == std::string::npos )
//	{
//		System::Print("Atlas meta data path %s extension is not .xml!", atlasMetaData.path.c_str());
//		return;
//	}
//
//	std::string line;
//
//	while ( std::getline(metaStream, line) )
//	{
//		if ( line.find("<Image Name") == std::string::npos )
//		{
//			continue;
//		}
//
//		std::vector< std::string > tokens;
//		Tokenize(line, tokens, "\"");
//		bool found = false;
//
//		for ( std::size_t t = 0; t < tokens.size(); ++t )
//		{
//			if ( tokens[t].find("Name") != std::string::npos && tokens[t + 1] == textureName )
//			{
//				found = true;
//			}
//
//			if ( !found )
//			{
//				continue;
//			}
//
//			if ( tokens[t].find("XPos") != std::string::npos )
//			{
//				scaleOffset.z = std::stoi(tokens[t + 1]) / static_cast<float>(width);
//			}
//			else if ( tokens[t].find("YPos") != std::string::npos )
//			{
//				scaleOffset.w = std::stoi(tokens[t + 1]) / static_cast<float>(height);
//			}
//			else if ( tokens[t].find("Width") != std::string::npos )
//			{
//				const int w = std::stoi(tokens[t + 1]);
//				scaleOffset.x = 1.0f / (static_cast<float>(width) / static_cast<float>(w));
//				width = w;
//			}
//			else if ( tokens[t].find("Height") != std::string::npos )
//			{
//				const int h = std::stoi(tokens[t + 1]);
//				scaleOffset.y = 1.0f / (static_cast<float>(height) / static_cast<float>(h));
//				height = h;
//			}
//		}
//
//		if ( found )
//		{
//			return;
//		}
//	}
//}