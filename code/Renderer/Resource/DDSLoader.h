#pragma once

namespace DDSLoader
{
	enum class LoadResult { Success, UnknownPixelFormat, FileNotFound };

	/**
	Loads a .dds file.

	OpenGL renderer:
	Stores the image data into the currently bound texture.
	Texture must be bound before calling this method.

	\param fileContents Contents of .dds file.
	\param cubeMapFace Cube map face index 1-6. For 2D textures use 0.
	\param outWidth Stores the width of the texture in pixels.
	\param outHeight Stores the height of the texture in pixels.
	\param outOpaque Stores info about alpha channel.
	\return Load result.
	*/
	LoadResult Load(const std::vector<uint8_t> &fileContents, int cubeMapFace, int &outWidth, int &outHeight, bool &outOpaque);
}