// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "VertexBuffer.h"

//-----------------------------------------------------------------------
VertexBuffer::VertexBuffer(size_t vertexCountIn, size_t strideInBytesIn, BufferUsage bufferUsageIn, const void *vertices)
	: m_vertexCount(vertexCountIn)
	, m_strideInBytes(strideInBytesIn)
	, m_bufferUsage(bufferUsageIn)
{
	Assert(m_vertexCount > 0);
	Assert(m_strideInBytes > 0);

	const uint32_t sizeInBytes = m_vertexCount * m_strideInBytes;
	m_nativeVertexBuffer = std::make_unique<BufferGL4>(BufferBindMode::VertexBuffer, sizeInBytes, m_bufferUsage, vertices);
	Assert(m_nativeVertexBuffer);
}
//-----------------------------------------------------------------------
void VertexBuffer::Bind()
{
	Assert(m_nativeVertexBuffer->bufferObject);
	glBindBuffer(m_nativeVertexBuffer->type, m_nativeVertexBuffer->bufferObject);
}
//-----------------------------------------------------------------------