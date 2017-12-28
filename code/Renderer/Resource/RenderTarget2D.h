#pragma once

#include "Texture2D.h"

enum class DepthFormat : uint8_t
{
	Depth16,
	Depth24,			// TODO
	Depth24Stencil8,
	Depth32,
	Depth32FStencil8,
	None
};

class RenderTarget2D : Noncopyable
{
public:
	RenderTarget2D(uint32_t pixelWidth, uint32_t pixelHeight, int32_t levelCount, DataFormat format, DepthFormat depthStencilFormat, int32_t multiSampleCount);
	~RenderTarget2D();

	uint32_t GetWidth() const;
	uint32_t GetHeight() const;

	void Bind(uint32_t attachmentPoint);
	void Unbind(uint32_t attachmentPoint);

	void BindTexture(uint32_t index);

private:
	uint32_t m_pixelWidth;
	uint32_t m_pixelHeight;
	int32_t m_levelCount;
	DataFormat m_format;
	DepthFormat m_depthStencilFormat;

	Texture2D m_texture;
	GLuint m_depthRenderBuffer = 0;
	GLuint m_frameBuffer;

	GLenum m_textureTarget;
	bool m_generateMipmap;
	bool m_multiSampleEnabled;
};