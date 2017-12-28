// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "VertexFormat.h"

//--------------------------------------------------------------------
GLenum toGLDataType(DataType dataType)
{
	switch ( dataType )
	{
	case DataType::Int8: return GL_BYTE;
	case DataType::UInt8: return GL_UNSIGNED_BYTE;
	case DataType::Int16: return GL_SHORT;
	case DataType::UInt16: return GL_UNSIGNED_SHORT;
	case DataType::Int32: return GL_INT;
	case DataType::UInt32: return GL_UNSIGNED_INT;
	case DataType::Float: return GL_FLOAT;
	case DataType::Double: return GL_DOUBLE;
	}
	ParameterFailed("DataType");
}
//--------------------------------------------------------------------
VertexAttribute::VertexAttribute(const VectorType vectorType, uint32_t instanceDivisor)
	: vectorType(vectorType)
	, instanceDivisor(instanceDivisor)
{
}
//--------------------------------------------------------------------
uint32_t VertexAttribute::GetSize() const
{
	return VectorTypeSize(vectorType);
}
//--------------------------------------------------------------------
VertexFormat::VertexFormat()
{
	glGenVertexArrays(1, &m_vao);
}
//--------------------------------------------------------------------
VertexFormat::~VertexFormat()
{
	glDeleteVertexArrays(1, &m_vao);
}
//--------------------------------------------------------------------
void VertexFormat::PushAttribute(const VectorType vectorType, uint32_t instanceDivisor)
{
	m_attributes.push_back( {vectorType, instanceDivisor} );
	auto &attr = m_attributes.back();

	if ( m_attributes.size() > 1 )
	{
		const auto &prevAttr = m_attributes[m_attributes.size() - 2];
		attr.offset = prevAttr.offset + prevAttr.GetSize();
	}
	else
		attr.offset = 0;

	m_stride += attr.GetSize();
}
//--------------------------------------------------------------------
void VertexFormat::Build(std::shared_ptr<VertexBuffer> vb)
{
	glBindVertexArray(m_vao);
	vb->Bind();
	for ( size_t i = 0; i < m_attributes.size(); i++)
		buildVertexAttribute(m_attributes[i], m_attributes[i].offset, m_stride, i);
	glBindVertexArray(0);
}
//--------------------------------------------------------------------
void VertexFormat::Build(const std::vector<std::shared_ptr<VertexBuffer>> &vbs)
{
	Assert(vbs.size() == m_attributes.size());

	glBindVertexArray(m_vao);
	size_t i = 0;
	for ( const auto &it : vbs )
	{
		it->Bind();
		buildVertexAttribute(m_attributes[i], 0, 0, i);
		i++;
	}

	glBindVertexArray(0);
}
//--------------------------------------------------------------------
void VertexFormat::Bind()
{
	glBindVertexArray(m_vao);
}
//--------------------------------------------------------------------
void VertexFormat::buildVertexAttribute(const VertexAttribute &attribute, GLsizeiptr offsetPtrSized, uint32_t stride, uint32_t index)
{
	glEnableVertexAttribArray(index);

	if ( attribute.instanceDivisor > 0 ) 
		glVertexAttribDivisor(index, attribute.instanceDivisor);

	DataType dataType = DataType::Float;
	uint8_t components = 0;
	VectorTypeFormat(attribute.vectorType, dataType, components);

	if ( dataType != DataType::Float && dataType != DataType::Double )
		glVertexAttribIPointer(index, components, toGLDataType(dataType), stride, reinterpret_cast<const void*>(offsetPtrSized));
	else
		glVertexAttribPointer(index, components, toGLDataType(dataType), GL_FALSE, stride, reinterpret_cast<const void*>(offsetPtrSized));
}
//--------------------------------------------------------------------
