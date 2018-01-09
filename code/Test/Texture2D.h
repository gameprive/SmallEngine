#pragma once

#include "TextureBase.h"

/// 2D texture.
class nTexture2D : public TextureBase
{
public:
	/// Gets a default texture that is always available after System::LoadBuiltinAssets().
	static nTexture2D* GetDefaultTexture();

	/// \param textureData Texture image data. File format must be dds, png, tga, jpg, bmp or bmp.
	/// \param wrap Wrap mode.
	/// \param filter Filter mode.
	/// \param mipmaps Mipmaps
	/// \param colorSpace Color space.
	/// \param anisotropy Anisotropy. Value range is 1-16 depending on support. On Metal the value is bucketed into 1, 2, 4, 8 and 16.
	void Load(const std::string &path, const std::vector<uint8_t>& textureData, TextureWrap wrap, TextureFilter filter, Mipmaps mipmaps, ColorSpace colorSpace, Anisotropy anisotropy);

	/// \param atlasTextureData Atlas texture image data. File format must be dds, png, tga, jpg, bmp or bmp.
	/// \param atlasMetaData Atlas metadata. Format is Ogre/CEGUI. Example atlas tool: Texture Packer.
	/// \param textureName Name of the texture in atlas.
	/// \param wrap Wrap mode.
	/// \param filter Filter mode.
	/// \param colorSpace Color space.
	/// \param anisotropy Anisotropy. Value range is 1-16 depending on support. On Metal the value is bucketed into 1, 2, 4, 8 and 16.
	//void LoadFromAtlas(const std::vector<uint8_t>& atlasTextureData, const std::vector<uint8_t>& atlasMetaData, const char* textureName, TextureWrap wrap, TextureFilter filter, ColorSpace colorSpace, Anisotropy anisotropy);

	/// \param imageData Raw pixel data
	/// \param width Width in pixels
	/// \param height Height in pixels
	/// \param channels Channels in the image, must be 3 or 4
	/// \param debugName Null-terminated string of texture's debug name that is visible in graphics debugging tools
	void LoadFromData(const void* imageData, int width, int height, int channels, const char* debugName);

	/// Destroys all textures. Called internally at exit.
	static void DestroyTextures();

private:
	void LoadDDS(const char* path);
	void LoadSTB(const std::string &path, const std::vector<uint8_t>& textureData);
};