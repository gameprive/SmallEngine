#pragma once

enum class BufferUsage : uint8_t
{
	Immutable,
	Dynamic,
};

enum class BufferBindMode
{
	ConstantBuffer,
	IndexBuffer,
	VertexBuffer,
};

class BufferGL4
{
public:
	BufferGL4(BufferBindMode type, size_t sizeInBytes, BufferUsage bufferUsage, const void *sourceData);
	~BufferGL4();

	void SetData(size_t offsetInBytes, size_t sizeInBytes, const void *source);

	GLuint bufferObject = 0;
	GLenum type = 0;
};