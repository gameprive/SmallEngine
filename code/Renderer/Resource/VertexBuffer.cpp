// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "VertexBuffer.h"

//--------------------------------------------------------------------
VertexBuffer::VertexBuffer(const void *vertices, uint32_t vertexCount, size_t sizeInBytes, BufferUsage bufferUsage)
	: Buffer(BufferType::Vertex, vertices, sizeInBytes, bufferUsage)
	, m_vertexCount(vertexCount)
{
}
//--------------------------------------------------------------------
void VertexBuffer::Bind()
{
	glBindBuffer(m_type, m_buffer);
}
//--------------------------------------------------------------------
size_t VertexBuffer::GetVertexCount() const
{
	return m_vertexCount;
}
//--------------------------------------------------------------------
