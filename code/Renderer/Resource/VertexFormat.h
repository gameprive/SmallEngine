#pragma once

#include "Renderer/Format.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

struct VertexAttribute
{
	VertexAttribute(const VectorType vectorType, uint32_t instanceDivisor = 0);

	uint32_t GetSize() const;
		
	VectorType vectorType = VectorType::Float4;
	uint32_t instanceDivisor = 0;
	uint32_t offset = 0;				
};

class VertexFormat : Noncopyable
{
public:
	VertexFormat();
	~VertexFormat();

	void PushAttribute(const VectorType vectorType, uint32_t instanceDivisor = 0);
	
	void Build(VertexBuffer *vb);
	void Build(std::shared_ptr<VertexBuffer> vb);
	void Build(const std::vector<std::shared_ptr<VertexBuffer>> &vbs);

	void Build(VertexBuffer *vb, IndexBuffer *ib);

	void Bind();

private:
	void buildVertexAttribute(const VertexAttribute &attribute, GLsizeiptr offsetPtrSized, uint32_t stride, uint32_t index);

	std::vector<VertexAttribute> m_attributes;
	uint32_t m_stride = 0;
	GLuint m_vao = 0;
};