﻿// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "VertexBuffer.h"

//-----------------------------------------------------------------------
VertexBuffer::VertexBuffer(const void *vertices, size_t vertexCount, size_t strideInBytes, BufferUsage bufferUsage)
	: Buffer(BufferType::Vertex, vertices, vertexCount * strideInBytes, bufferUsage)
	, m_vertexCount(vertexCount)
{
}
//-----------------------------------------------------------------------
VertexBuffer::VertexBuffer(size_t vertexCount, size_t strideInBytes, BufferUsage bufferUsage)
	: Buffer(BufferType::Vertex, nullptr, vertexCount * strideInBytes, bufferUsage)
	, m_vertexCount(vertexCount)
{
}
//-----------------------------------------------------------------------
void VertexBuffer::Bind()
{
	Assert(m_bufferObject);
	glBindBuffer(m_type, m_bufferObject);
}
//-----------------------------------------------------------------------