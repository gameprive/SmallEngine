#pragma once

#include "Buffer.h"

class ConstantBuffer : public Buffer
{
public:
	ConstantBuffer(size_t sizeInBytes, BufferUsage bufferUsage);
	ConstantBuffer(const void *sourceData, size_t sizeInBytes, BufferUsage bufferUsage);

	void Bind(uint8_t slotIndex);

	template <typename T>
	void SetData(const T &value)
	{
		SetData(static_cast<const void*>(&value), sizeof(value));
	}

private:
	uint32_t m_sizeInBytes;
};