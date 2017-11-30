#pragma once

#include "Buffer.h"

enum class IndexElementSize : uint8_t
{
	Bit16,
	Bit32,
};

class IndexBuffer : public Buffer
{
public:
	IndexBuffer(IndexElementSize elementSize, const void *indices, size_t indexCount, BufferUsage bufferUsage);
	IndexBuffer(IndexElementSize elementSize, size_t indexCount, BufferUsage bufferUsage);

	void Bind();

	size_t GetIndexCount() const;

private:
	uint32_t m_indexCount;
	IndexElementSize m_elementSize;
};