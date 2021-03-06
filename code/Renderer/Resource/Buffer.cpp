﻿// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "Buffer.h"
#include "System/RenderDevice.h"

//--------------------------------------------------------------------
// TODO: Buffer Storage
// https://ferransole.wordpress.com/2014/06/08/persistent-mapped-buffers/
// http://steps3d.narod.ru/tutorials/buffer-storage-tutorial.html
// http://www.bfilipek.com/2015/01/persistent-mapped-buffers-benchmark.html
// Unreal Engine 4
//if ( RenderDevice::SupportsBufferStorage() )
//{
//	const GLbitfield flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
//	glBufferStorage(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices, flags);
//}
//else
//{
//	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);
//}
//--------------------------------------------------------------------
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
	ParameterFailed("toGLBufferType");
}
//--------------------------------------------------------------------
GLenum toGLBufferUsage(BufferUsage bufferUsage)
{
	switch ( bufferUsage )
	{
	case BufferUsage::Dynamic: return GL_DYNAMIC_DRAW;
	case BufferUsage::Immutable: return GL_STATIC_DRAW;
	}
	ParameterFailed("toGLBufferUsage");
}
//--------------------------------------------------------------------
GLenum toGLBufferAccess(BufferAccess access)
{
	switch ( access )
	{
	case BufferAccess::ReadOnly: return GL_READ_ONLY;
	case BufferAccess::WriteOnly: return GL_WRITE_ONLY;
	case BufferAccess::ReadWriteOnly: return GL_READ_WRITE;
	}
	ParameterFailed("toGLBufferAccess");
}
//--------------------------------------------------------------------
Buffer::Buffer(BufferType type, const void *sourceData, size_t sizeInBytes, BufferUsage bufferUsage)
{
	Assert(sizeInBytes > 0);

	m_type = toGLBufferType(type);

	if ( !sourceData ) bufferUsage = BufferUsage::Dynamic;
	
	glGenBuffers(1, &m_buffer);

	glBindBuffer(m_type, m_buffer);
	glBufferData(m_type, sizeInBytes, sourceData, toGLBufferUsage(bufferUsage));
	glBindBuffer(m_type, 0);
}
//--------------------------------------------------------------------
Buffer::~Buffer()
{
	if ( m_buffer )
	{
		glBindBuffer(m_type, 0);
		glDeleteBuffers(1, &m_buffer);
		m_buffer = 0;
	}
}
//--------------------------------------------------------------------
void Buffer::SetData(const void *source, size_t sizeInBytes)
{
	SetData(0, source, sizeInBytes);
}
//--------------------------------------------------------------------
void Buffer::SetData(size_t offsetInBytes, const void *source, size_t sizeInBytes)
{
	Assert(source != nullptr);
	Assert(sizeInBytes > 0);
	glBindBuffer(m_type, m_buffer);
	glBufferSubData(m_type, offsetInBytes, sizeInBytes, source);
	glBindBuffer(m_type, 0);
}
//--------------------------------------------------------------------
void* Buffer::Map(BufferAccess access)
{
	glBindBuffer(m_type, m_buffer);
	return glMapBuffer(m_type, toGLBufferAccess(access));
}
//--------------------------------------------------------------------
bool Buffer::Unmap()
{
	const bool ret = (glUnmapBuffer(m_type) == GL_TRUE);
	glBindBuffer(m_type, 0);
	return ret;
}
//--------------------------------------------------------------------
void Buffer::SetDebugName(const char *name)
{
	RenderDevice::LabelObject(GL_BUFFER, m_buffer, name);
}
//--------------------------------------------------------------------
