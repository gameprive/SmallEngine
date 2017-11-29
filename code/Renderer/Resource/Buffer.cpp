// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "Buffer.h"

//-----------------------------------------------------------------------
GLenum toGLBufferType(BufferBindMode mode)
{
	switch ( mode )
	{
	case BufferBindMode::ConstantBuffer: return GL_UNIFORM_BUFFER;
	case BufferBindMode::IndexBuffer: return GL_ELEMENT_ARRAY_BUFFER;
	case BufferBindMode::VertexBuffer: return GL_ARRAY_BUFFER;
	}
	return GL_ARRAY_BUFFER;
}
//-----------------------------------------------------------------------
GLenum toGLBufferUsage(BufferUsage bufferUsage)
{
	switch ( bufferUsage )
	{
	case BufferUsage::Dynamic: return GL_DYNAMIC_DRAW;
	case BufferUsage::Immutable: return GL_STATIC_DRAW;
	}
	return GL_STATIC_DRAW;
}
//-----------------------------------------------------------------------
GLenum toGLBufferBind(BufferBindMode mode)
{
	switch ( mode )
	{
	case BufferBindMode::ConstantBuffer: return GL_UNIFORM_BUFFER_BINDING;
	case BufferBindMode::IndexBuffer: return GL_ELEMENT_ARRAY_BUFFER_BINDING;
	case BufferBindMode::VertexBuffer: return GL_ARRAY_BUFFER_BINDING;
	}
	return GL_ARRAY_BUFFER_BINDING;
}
//-----------------------------------------------------------------------
BufferGL4::BufferGL4(BufferBindMode setType, size_t sizeInBytes, BufferUsage bufferUsage, const void *sourceData)
{
	Assert(sizeInBytes > 0);
	if ( !sourceData ) bufferUsage = BufferUsage::Dynamic;
	type = toGLBufferType(setType);

	glGenBuffers(1, &bufferObject);
	Assert(bufferObject);
	glBindBuffer(type, bufferObject);
	glBufferData(type, sizeInBytes, sourceData, toGLBufferUsage(bufferUsage));
	glBindBuffer(type, 0);
}
//-----------------------------------------------------------------------
BufferGL4::~BufferGL4()
{
	if ( bufferObject )
	{
		glBindBuffer(type, 0);
		glDeleteBuffers(1, &bufferObject);
	}
}
//-----------------------------------------------------------------------
void BufferGL4::SetData(size_t offsetInBytes, size_t sizeInBytes, const void *source)
{
	Assert(source != nullptr);
	Assert(sizeInBytes > 0);
	glBindBuffer(type, bufferObject);
	glBufferSubData(type, offsetInBytes, sizeInBytes, source);
	glBindBuffer(type, 0);
}
//-----------------------------------------------------------------------