#pragma once

#include "Buffer.h"

class VertexBuffer : public Buffer
{
public:
	VertexBuffer(const void *vertices, size_t vertexCount, size_t strideInBytes, BufferUsage bufferUsage);
	VertexBuffer(size_t vertexCount, size_t strideInBytes, BufferUsage bufferUsage);
	
	void Bind();

	size_t GetVertexCount() const;

private:
	uint32_t m_vertexCount = 0;
};