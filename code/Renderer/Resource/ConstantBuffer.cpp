// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "ConstantBuffer.h"

//--------------------------------------------------------------------
ConstantBuffer::ConstantBuffer(size_t sizeInBytes)
	: Buffer(BufferType::Constant, nullptr, sizeInBytes, BufferUsage::Dynamic)
	, m_sizeInBytes(sizeInBytes)
{
}
//--------------------------------------------------------------------
ConstantBuffer::ConstantBuffer(const void *sourceData, size_t sizeInBytes)
	: Buffer(BufferType::Constant, sourceData, sizeInBytes, BufferUsage::Dynamic)
	, m_sizeInBytes(sizeInBytes)
{
}
//--------------------------------------------------------------------
void ConstantBuffer::Bind(uint8_t slotIndex)
{
	glBindBufferBase(GL_UNIFORM_BUFFER, slotIndex, m_buffer);
}
//--------------------------------------------------------------------
