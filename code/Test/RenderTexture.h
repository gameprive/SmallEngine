#pragma once

#include "TextureBase.h"

/// Render texture. Can be 2D or cube map.
class RenderTexture : public TextureBase
{
public:
	/// Data type.
	enum class DataType
	{
		UByte,
		Float,
		R32G32
	};

	/// Destroys graphics API objects.
	static void DestroyTextures();

	/// \return Data type.
	DataType GetDataType() const { return dataType; }

	/// \param width Width.
	/// \param height Height.
	/// \param dataType Data type.
	/// \param wrap Wrapping mode.
	/// \param filter Filtering mode.
	/// \param debugName Debug name that is visible in graphics debugging tools.
	void Create2D(int width, int height, DataType dataType, TextureWrap wrap, TextureFilter filter, const char* debugName);

	/// \param dimension Dimension.
	/// \param dataType Data type.
	/// \param wrap Wrapping mode.
	/// \param filter Filtering mode.
	/// \param debugName Debug name that is visible in graphics debugging tools.
	void CreateCube(int dimension, DataType dataType, TextureWrap wrap, TextureFilter filter, const char* debugName);

	/// \return FBO.
	unsigned GetFBO() const { return fboId; }

private:
	unsigned rboId = 0;
	unsigned fboId = 0;
	DataType dataType = DataType::UByte;
};