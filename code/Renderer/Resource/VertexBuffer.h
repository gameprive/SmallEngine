#pragma once

#include "Buffer.h"

class VertexBuffer : Noncopyable
{
public:
	VertexBuffer(size_t vertexCount, size_t strideInBytes, BufferUsage bufferUsage, const void *vertices);

	void Bind();

private:
	std::unique_ptr<BufferGL4> m_nativeVertexBuffer;
	uint32_t m_vertexCount;
	uint32_t m_strideInBytes;
	BufferUsage m_bufferUsage;
};