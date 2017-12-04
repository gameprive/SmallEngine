// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "RenderTarget2D.h"

//-----------------------------------------------------------------------
GLenum toDepthStencilFormat(DepthFormat depthFormat)
{
	switch ( depthFormat )
	{
	case DepthFormat::Depth16: return GL_DEPTH_COMPONENT16;
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
{
	if ( depthStencilFormat != DepthFormat::None )
	{
		Assert(pixelWidth > 0);
		Assert(pixelHeight > 0);

		glGenRenderbuffers(1, &m_renderBuffer);
		Assert(m_renderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, toDepthStencilFormat(depthStencilFormat), pixelWidth, pixelHeight);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}
}
//-----------------------------------------------------------------------
RenderTarget2D::~RenderTarget2D()
{
	if ( m_renderBuffer ) 
		glDeleteRenderbuffers(1, &m_renderBuffer);
}
//-----------------------------------------------------------------------
void RenderTarget2D::BindToFramebuffer(uint32_t attachmentPoint)
{
	const GLenum textureTarget = (m_multiSampleEnabled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D);
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentPoint, textureTarget, m_texture.m_textureObject, 0);
}
//-----------------------------------------------------------------------
void RenderTarget2D::UnbindFromFramebuffer(uint32_t attachmentPoint)
{
	const GLenum textureTarget = (m_multiSampleEnabled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D);
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentPoint, textureTarget, 0, 0);
	if ( m_generateMipmap ) m_texture.GenerateMipmap();
}
//-----------------------------------------------------------------------
void RenderTarget2D::BindDepthStencilBuffer()
{
	if ( m_renderBuffer )
	{
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_renderBuffer);
	}
}
//-----------------------------------------------------------------------
void RenderTarget2D::BindTexture(uint32_t index)
{
	m_texture.Bind(index);
}
//-----------------------------------------------------------------------