// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "Buffer.h"

//-----------------------------------------------------------------------
GLenum toGLBufferType(BufferType mode)
{
	switch ( mode )
	{
	case BufferType::Vertex: return GL_ARRAY_BUFFER;
	case BufferType::Index: return GL_ELEMENT_ARRAY_BUFFER;
	case BufferType::Constant: return GL_UNIFORM_BUFFER;
	case BufferType::Storage: return GL_SHADER_STORAGE_BUFFER;
	case BufferType::StreamOutput: return GL_TRANSFORM_FEEDBACK_BUFFER;
	}
	ParameterFailed("BufferType");
}
//-----------------------------------------------------------------------
GLenum toGLBufferUsage(BufferUsage bufferUsage)
{
	switch ( bufferUsage )
	{
	case BufferUsage::Dynamic: return GL_DYNAMIC_DRAW;
	case BufferUsage::Immutable: return GL_STATIC_DRAW;
	}
	ParameterFailed("BufferUsage");
}
//-----------------------------------------------------------------------
GLenum toGLBufferAccess(BufferAccess access)
{
	switch ( access )
	{
	case BufferAccess::Read: return GL_READ_ONLY;
	case BufferAccess::Write: return GL_WRITE_ONLY;
	case BufferAccess::ReadWrite: return GL_READ_WRITE;
	}
	ParameterFailed("BufferAccess");
}
//-----------------------------------------------------------------------
Buffer::Buffer(BufferType type, const void *sourceData, size_t sizeInBytes, BufferUsage bufferUsage)
{
	Assert(sizeInBytes > 0);
	if ( !sourceData ) bufferUsage = BufferUsage::Dynamic;
	m_type = toGLBufferType(type);

	glGenBuffers(1, &m_bufferObject);
	Assert(m_bufferObject);
	Assert(sizeInBytes > 0);
	glBindBuffer(m_type, m_bufferObject);
	glBufferData(m_type, sizeInBytes, sourceData, toGLBufferUsage(bufferUsage));
	glBindBuffer(m_type, 0);
}
//-----------------------------------------------------------------------
Buffer::~Buffer()
{
	if ( m_bufferObject )
	{
		glBindBuffer(m_type, 0);
		glDeleteBuffers(1, &m_bufferObject);
	}
}
//-----------------------------------------------------------------------
void Buffer::SetData(const void *source, size_t sizeInBytes)
{
	Assert(source != nullptr);
	Assert(sizeInBytes > 0);
	glBindBuffer(m_type, m_bufferObject);
	glBufferSubData(m_type, 0, sizeInBytes, source);
	glBindBuffer(m_type, 0);
}
//-----------------------------------------------------------------------
void Buffer::SetData(size_t offsetInBytes, const void *source, size_t sizeInBytes)
{
	Assert(source != nullptr);
	Assert(sizeInBytes > 0);
	glBindBuffer(m_type, m_bufferObject);
	glBufferSubData(m_type, offsetInBytes, sizeInBytes, source);
	glBindBuffer(m_type, 0);
}
//-----------------------------------------------------------------------
void* Buffer::Map(BufferAccess access)
{
	glBindBuffer(m_type, m_bufferObject);
	return glMapBuffer(m_type, toGLBufferAccess(access));
}
//-----------------------------------------------------------------------
bool Buffer::Unmap()
{
	const bool ret = (glUnmapBuffer(m_type) == GL_TRUE);
	glBindBuffer(m_type, 0);
	return ret;
}
//-----------------------------------------------------------------------