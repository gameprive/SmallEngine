// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "ConstantBuffer.h"

//-----------------------------------------------------------------------
ConstantBuffer::ConstantBuffer(size_t sizeInBytes, BufferUsage bufferUsage)
	: Buffer(BufferType::Constant, nullptr, sizeInBytes, bufferUsage)
	, m_sizeInBytes(sizeInBytes)
{
}
//-----------------------------------------------------------------------
ConstantBuffer::ConstantBuffer(const void *sourceData, size_t sizeInBytes, BufferUsage bufferUsage)
	: Buffer(BufferType::Constant, sourceData, sizeInBytes, bufferUsage)
	, m_sizeInBytes(sizeInBytes)
{
}
//-----------------------------------------------------------------------
void ConstantBuffer::Bind(uint8_t slotIndex)
{
	Assert(m_bufferObject);
	glBindBufferBase(GL_UNIFORM_BUFFER, slotIndex, m_bufferObject);
}
//-----------------------------------------------------------------------