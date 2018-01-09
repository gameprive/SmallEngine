// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "RenderTexture.h"
#include "HasExtension.h"

static void GetGLFormat(RenderTexture::DataType dataType, GLenum& outInternalFormat, GLenum& outExternalFormat)
{
	if ( dataType == RenderTexture::DataType::UByte )
	{
		outInternalFormat = GL_RGBA;
		outExternalFormat = GL_RGBA;
	}
	else if ( dataType == RenderTexture::DataType::Float )
	{
		outInternalFormat = GL_RGBA32F;
		outExternalFormat = GL_RGBA;
	}
	else if ( dataType == RenderTexture::DataType::R32G32 )
	{
		outInternalFormat = GL_RG32F;
		outExternalFormat = GL_RGBA;
	}
	else
	{
		LogError() << "Unhandled texture format";
		outInternalFormat = GL_RGBA;
		outExternalFormat = GL_RGBA;
	}
}

void RenderTexture::Create2D(int aWidth, int aHeight, DataType aDataType, TextureWrap aWrap, TextureFilter aFilter, const char* debugName)
{
	if ( aWidth <= 0 || aHeight <= 0 )
	{
		LogError() << "Render texture has invalid dimension!";
		return;
	}

	width = aWidth;
	height = aHeight;
	wrap = aWrap;
	filter = aFilter;
	isCube = false;
	isRenderTexture = true;
	dataType = aDataType;

	glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_2D, handle);

	if ( HasExtension("GL_KHR_debug") )
	{
		glObjectLabel(GL_TEXTURE, handle, -1, debugName);
	}

	const float borderColor[] = { 0, 0, 0, 1 };
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter == TextureFilter::Nearest ? GL_NEAREST : (mipmaps == Mipmaps::Generate ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter == TextureFilter::Nearest ? GL_NEAREST : GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap == TextureWrap::Repeat ? GL_REPEAT : GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap == TextureWrap::Repeat ? GL_REPEAT : GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	GLenum internalFormat, externalFormat;
	GetGLFormat(dataType, internalFormat, externalFormat);
	const GLenum fmt = (dataType == DataType::Float || dataType == DataType::R32G32) ? GL_FLOAT : GL_UNSIGNED_BYTE;

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, externalFormat, fmt, nullptr);
	ErrorCheck("Load Texture2D");

	glBindTexture(GL_TEXTURE_2D, 0);

	// Creates a renderbuffer object to store depth info.
	glGenRenderbuffers(1, &rboId);
	glBindRenderbuffer(GL_RENDERBUFFER, rboId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// Creates the Frame Buffer Object.
	glGenFramebuffers(1, &fboId);
	glBindFramebuffer(GL_FRAMEBUFFER, fboId);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, handle, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboId);
	// FIXME: Set this only in editor if this causes problems. This cannot be systemFBO because it hangs the editor.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	ErrorCheckFBO();
	ErrorCheck("CreateRenderTexture2D end");
}

void RenderTexture::CreateCube(int aDimension, DataType aDataType, TextureWrap aWrap, TextureFilter aFilter, const char* debugName)
{
	if ( aDimension <= 0 )
	{
		LogError() << "Render texture has invalid dimension!";
		return;
	}

	width = height = aDimension;
	wrap = aWrap;
	filter = aFilter;
	isCube = true;
	isRenderTexture = true;
	dataType = aDataType;

	glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_CUBE_MAP, handle);

	if ( HasExtension("GL_KHR_debug") )
	{
		glObjectLabel(GL_TEXTURE, handle, -1, debugName);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, filter == TextureFilter::Nearest ? GL_NEAREST : (mipmaps == Mipmaps::Generate ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, filter == TextureFilter::Nearest ? GL_NEAREST : GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrap == TextureWrap::Repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrap == TextureWrap::Repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrap == TextureWrap::Repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);

	// Creates a renderbuffer object to store depth info.
	glGenRenderbuffers(1, &rboId);
	glBindRenderbuffer(GL_RENDERBUFFER, rboId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, aDimension, aDimension);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glGenFramebuffers(1, &fboId);
	glBindFramebuffer(GL_FRAMEBUFFER, fboId);

	const GLenum externalFormat = GL_RGBA;
	const GLenum internalFormat = GL_RGBA8;
	const GLenum dataTypeGL = dataType == DataType::Float ? GL_FLOAT : GL_UNSIGNED_BYTE;

	for ( int i = 0; i < 6; ++i )
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat,
			aDimension, aDimension, 0, externalFormat,
			dataTypeGL, nullptr);
		ErrorCheck("Load Texture Cube");
	}

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	ErrorCheckFBO();
	ErrorCheck("CreateRenderTextureCube end");
}