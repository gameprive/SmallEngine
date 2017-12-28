// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "RenderDevice.h"

//-----------------------------------------------------------------------
void RenderDevice::Init()
{
	const auto glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
	const auto glslVersion = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));
	const auto vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
	const auto renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
	
	Log infoLog(LevelLog::Info);
	infoLog << ":\n";
	infoLog << "\n\tGL version:   " << glVersion;
	infoLog << "\n\tGL vendor:    " << vendor;
	infoLog << "\n\tGL renderer:  " << renderer;
	infoLog << "\n\tGLSL version: " << glslVersion;
	
	glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &m_deviceMaxAnisotropy);
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &m_maxTextureSlot);
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &m_maxVertexConstantBuffers);
	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &m_maxPixelConstantBuffers);

	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &m_maxVertexUniformVectors);

	infoLog << "\n";
	infoLog << "\n\tMax Anisotropy:               " << m_deviceMaxAnisotropy;
	infoLog << "\n\tMax Texture Slot:             " << m_maxTextureSlot;
	infoLog << "\n\tMax Vertex Constant Buffer:   " << m_maxVertexConstantBuffers;
	infoLog << "\n\tMax Pixel Constant Buffer:    " << m_maxPixelConstantBuffers;

	infoLog << "\n\tMax Vertex Constant Vectors:  " << m_maxVertexUniformVectors;
}
//-----------------------------------------------------------------------
int32_t RenderDevice::GetMaxAnisotropy() const
{
	return m_deviceMaxAnisotropy;
}
//-----------------------------------------------------------------------
int32_t RenderDevice::GetMaxTextureSlot() const
{
	return m_maxTextureSlot;
}
//-----------------------------------------------------------------------
int32_t RenderDevice::GetMaxVertexConstantBuffer() const
{
	return m_maxVertexConstantBuffers;
}
//-----------------------------------------------------------------------
int32_t RenderDevice::GetMaxPixelConstantBuffer() const
{
	return m_maxPixelConstantBuffers;
}
//-----------------------------------------------------------------------