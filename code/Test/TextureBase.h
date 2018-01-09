#pragma once

#include "Math/Vector3.h"

/// Texture wrap controls behavior when coordinates are outside range 0-1. Repeat should not be used for atlased textures.
enum class TextureWrap
{
	Repeat,
	Clamp
};

/// Filter controls behavior when the texture is smaller or larger than its original size.
enum class TextureFilter
{
	Nearest,
	Linear
};

/// Mipmap usage.
enum class Mipmaps
{
	Generate,
	None
};

enum class ColorSpace
{
	RGB,
	SRGB
};

enum class Anisotropy
{
	k1,
	k2,
	k4,
	k8
};

std::string GetCacheHash(const std::string& path, TextureWrap wrap, TextureFilter filter, Mipmaps mipmaps, ColorSpace colorSpace, Anisotropy anisotropy);

/// Base class for textures.
class TextureBase
{
public:
	/// \return id.
	unsigned GetID() const { return handle; }

	/// \return True, if the texture is a cube map.
	bool IsCube() const { return isCube; }

	/// \return Path where this texture was loaded from, if it was loaded from a file.
	std::string GetPath() const { return path; }

	/// \return Color space.
	ColorSpace GetColorSpace() const { return colorSpace; }

	/// \return Anisotropy.
	Anisotropy GetAnisotropy() const { return anisotropy; }

	/// \return Width in pixels.
	int GetWidth() const { return width; }

	/// \return Width in pixels.
	int GetHeight() const { return height; }

	/// \return Wrapping mode
	TextureWrap GetWrap() const { return wrap; }

	/// \return Filtering mode
	TextureFilter GetFilter() const { return filter; }

	/// \return Mipmap usage.
	Mipmaps GetMipmaps() const { return mipmaps; }

	/// \return Scale and offset. x: scale x, y: scale y, z: offset x, w: offset y.
	const Vec4& GetScaleOffset() const { return scaleOffset; }

	/// \return True, if the texture does not contain an alpha channel.
	bool IsOpaque() const { return opaque; }

	/// \return True, if the texture is a RenderTexture.
	bool IsRenderTexture() const { return isRenderTexture; }

protected:
	/// Width in pixels.
	int width = 0;
	/// Height in pixels.
	int height = 0;
	/// Graphics API handle.
	unsigned handle = 0;
	/// Wrapping controls how coordinates outside 0-1 are interpreted.
	TextureWrap wrap = TextureWrap::Repeat;
	/// Filtering mode.
	TextureFilter filter = TextureFilter::Nearest;
	/// Scale (tiling) and offset.
	Vec4 scaleOffset{ 1, 1, 0, 0 };
	/// Mipmaps.
	Mipmaps mipmaps = Mipmaps::None;
	/// Mipmap count.
	int mipLevelCount = 1;
	/// Anisotropy.
	Anisotropy anisotropy = Anisotropy::k1;
	/// Color space.
	ColorSpace colorSpace = ColorSpace::RGB;
	/// Is the texture a cube map?
	bool isCube = false;

	/// Is the texture opaque.
	bool opaque = true;
	/// Is the texture a render texture.
	bool isRenderTexture = false;
	/// Path where this texture was loaded from, if it was loaded from a file.
	std::string path;
};