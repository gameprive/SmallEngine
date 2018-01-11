// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "RenderDevice.h"

//--------------------------------------------------------------------
int32_t deviceMaxAnisotropy;
int32_t maxTextureSlot;
int32_t uniformBufferOffsetAlignment;

bool m_debugContext;
//--------------------------------------------------------------------
void RenderDevice::Init()
{
	const auto glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
	const auto glslVersion = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));
	const auto vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
	const auto renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));

	m_debugContext = (glIsEnabled(GL_DEBUG_OUTPUT) != GL_FALSE) && SupportsKHRDebug();
	
	Log infoLog(LevelLog::Info);	
	infoLog << ":\n";
	infoLog << "\n\tGL version:   " << glVersion;
	infoLog << "\n\tGL vendor:    " << vendor;
	infoLog << "\n\tGL renderer:  " << renderer;
	infoLog << "\n\tGLSL version: " << glslVersion;
	
	glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &deviceMaxAnisotropy);
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTextureSlot);
	glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &uniformBufferOffsetAlignment);

	infoLog << "\n";
	infoLog << "\n\tMax Anisotropy:                  " << deviceMaxAnisotropy;
	infoLog << "\n\tMax Texture Slot:                " << maxTextureSlot;
	infoLog << "\n\tUniform Buffer Offset Alignment: " << uniformBufferOffsetAlignment;
	
}
//--------------------------------------------------------------------
int32_t RenderDevice::GetMaxAnisotropy()
{
	return deviceMaxAnisotropy;
}
//--------------------------------------------------------------------
int32_t RenderDevice::GetMaxTextureSlot()
{
	return maxTextureSlot;
}
//--------------------------------------------------------------------
bool RenderDevice::SupportsKHRDebug()
{
	return (GLEW_KHR_debug);
}
//--------------------------------------------------------------------
void RenderDevice::LabelObject(GLenum type, GLuint object, const char *name)
{
	if ( glObjectLabel && m_debugContext )
		glObjectLabel(type, object, -1, name);
}
//--------------------------------------------------------------------
void RenderDevice::PushGroupMarker(const char *name)
{
	if ( glPushDebugGroup && m_debugContext )
		glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, name);
}
//--------------------------------------------------------------------
void RenderDevice::PopGroupMarker()
{
	if ( glPopDebugGroup && m_debugContext )
		glPopDebugGroup();
}
//--------------------------------------------------------------------
