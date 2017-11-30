// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "IndexBuffer.h"

//-----------------------------------------------------------------------
size_t ToIndexElementOffsetBytes(IndexElementSize elementSize)
{
	switch ( elementSize )
	{
	case IndexElementSize::Bit32: return sizeof(int);
	case IndexElementSize::Bit16: return sizeof(short);
	};
	OpenGLParameterFailed("IndexElementOffsetBytes");
}
//-----------------------------------------------------------------------
IndexBuffer::IndexBuffer(IndexElementSize elementSize, const void *indices, size_t indexCount, BufferUsage bufferUsage)
	: Buffer(BufferType::Index, indices, indexCount * ToIndexElementOffsetBytes(elementSize), bufferUsage)
	, m_indexCount(indexCount)
	, m_elementSize(elementSize)
{
}
//-----------------------------------------------------------------------
IndexBuffer::IndexBuffer(IndexElementSize elementSize, size_t indexCount, BufferUsage bufferUsage)
	: Buffer(BufferType::Index, nullptr, indexCount * ToIndexElementOffsetBytes(elementSize), bufferUsage)
	, m_indexCount(indexCount)
	, m_elementSize(elementSize)
{
}
//-----------------------------------------------------------------------
void IndexBuffer::Bind()
{
	Assert(m_bufferObject);
	glBindBuffer(m_type, m_bufferObject);
}
//-----------------------------------------------------------------------
size_t IndexBuffer::GetIndexCount() const
{
	return m_indexCount;
}
//-----------------------------------------------------------------------