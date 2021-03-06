﻿#pragma once

#include "Buffer.h"

class VertexBuffer : public Buffer
{
public:
	VertexBuffer(const void *vertices, uint32_t vertexCount, size_t sizeInBytes, BufferUsage bufferUsage);
	
	void Bind();

	size_t GetVertexCount() const;

private:
	uint32_t m_vertexCount;
};