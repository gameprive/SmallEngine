#pragma once

enum class BufferUsage : uint8_t
{
	Immutable,
	Dynamic,
};

enum class BufferType : uint8_t
{
	Vertex,
	Index,
	Constant,		// Uniform Buffer Object
	Storage,		// Shader Storage Buffer Object
	StreamOutput	// Transform Feedback Buffer
};

enum class BufferAccess : uint8_t
{
	ReadOnly,
	WriteOnly,
	ReadWriteOnly
};

class Buffer : Noncopyable
{
public:
	~Buffer();

	void SetData(const void *source, size_t sizeInBytes);
	void SetData(size_t offsetInBytes, const void *source, size_t sizeInBytes);

	void* Map(BufferAccess access);
	bool Unmap();

protected:
	Buffer(BufferType type, const void *sourceData, size_t sizeInBytes, BufferUsage bufferUsage);

	GLuint m_bufferObject = 0;
	GLenum m_type = 0;
};