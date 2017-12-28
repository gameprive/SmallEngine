// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "RenderTarget2D.h"

//-----------------------------------------------------------------------
GLenum toColorAttachment(uint32_t index)
{
	static_assert(GL_COLOR_ATTACHMENT0 == (GL_COLOR_ATTACHMENT0 + 0), "");
	static_assert(GL_COLOR_ATTACHMENT1 == (GL_COLOR_ATTACHMENT0 + 1), "");
	static_assert(GL_COLOR_ATTACHMENT2 == (GL_COLOR_ATTACHMENT0 + 2), "");
	static_assert(GL_COLOR_ATTACHMENT3 == (GL_COLOR_ATTACHMENT0 + 3), "");
	static_assert(GL_COLOR_ATTACHMENT4 == (GL_COLOR_ATTACHMENT0 + 4), "");
	static_assert(GL_COLOR_ATTACHMENT5 == (GL_COLOR_ATTACHMENT0 + 5), "");
	static_assert(GL_COLOR_ATTACHMENT6 == (GL_COLOR_ATTACHMENT0 + 6), "");
	static_assert(GL_COLOR_ATTACHMENT7 == (GL_COLOR_ATTACHMENT0 + 7), "");

	Assert(index < 8);
	return static_cast<GLenum>(GL_COLOR_ATTACHMENT0 + index);
}
//-----------------------------------------------------------------------
GLenum toDepthStencilFormat(DepthFormat depthFormat)
{
	switch ( depthFormat )
	{
	case DepthFormat::Depth16: return GL_DEPTH_COMPONENT16;
	case DepthFormat::Depth24: return GL_DEPTH_COMPONENT24;
	case DepthFormat::Depth24Stencil8: return GL_DEPTH24_STENCIL8;
	case DepthFormat::Depth32: return GL_DEPTH_COMPONENT32;
	case DepthFormat::Depth32FStencil8: return GL_DEPTH32F_STENCIL8;
	case DepthFormat::None: break;
	}
	ParameterFailed("toDepthStencilFormat");
}
//-----------------------------------------------------------------------
RenderTarget2D::RenderTarget2D(uint32_t pixelWidth, uint32_t pixelHeight, int32_t levelCount, DataFormat format, DepthFormat depthStencilFormat, int32_t multiSampleCount)
	: m_texture(pixelWidth, pixelHeight, levelCount, format)
	, m_generateMipmap(levelCount > 1)
	, m_multiSampleEnabled(multiSampleCount > 1)
	, m_pixelWidth(pixelWidth)
	, m_pixelHeight(pixelHeight)
{

	m_textureTarget = (m_multiSampleEnabled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D);

	glGenFramebuffers(1, &m_frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

	if ( depthStencilFormat != DepthFormat::None )
	{	
		glGenRenderbuffers(1, &m_depthRenderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_depthRenderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, toDepthStencilFormat(depthStencilFormat), pixelWidth, pixelHeight);
		
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
//-----------------------------------------------------------------------
RenderTarget2D::~RenderTarget2D()
{
	glDeleteFramebuffers(1, &m_frameBuffer);
	if ( m_depthRenderBuffer )
		glDeleteRenderbuffers(1, &m_depthRenderBuffer);
}
//-----------------------------------------------------------------------
uint32_t RenderTarget2D::GetWidth() const
{
	return m_pixelWidth;
}
//-----------------------------------------------------------------------
uint32_t RenderTarget2D::GetHeight() const
{
	return m_pixelHeight;
}
//-----------------------------------------------------------------------
void RenderTarget2D::Bind(uint32_t attachmentPoint)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, toColorAttachment(attachmentPoint), m_textureTarget, m_texture.m_textureObject, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthRenderBuffer);
}
//-----------------------------------------------------------------------
void RenderTarget2D::Unbind(uint32_t attachmentPoint)
{
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, toColorAttachment(attachmentPoint), m_textureTarget, 0, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
//-----------------------------------------------------------------------
void RenderTarget2D::BindTexture(uint32_t index)
{
	m_texture.Bind(index);
}
//-----------------------------------------------------------------------