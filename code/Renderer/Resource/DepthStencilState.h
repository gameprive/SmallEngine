#pragma once

enum class ComparisonFunction : uint8_t
{
	Never,
	Always,
	Less,
	LessEqual,
	Equal,
	NotEqual,
	GreaterEqual,
	Greater,
};

enum class StencilOperation : uint8_t
{
	Keep,
	Zero,
	Replace,
	Increment,
	Decrement,
	IncrementSaturation,
	DecrementSaturation,
	Invert,
};

struct DepthStencilOperation
{
	StencilOperation StencilDepthBufferFail;
	StencilOperation StencilFail;
	StencilOperation StencilPass;
	ComparisonFunction StencilFunction;
};

struct DepthStencilDescription
{
	static DepthStencilDescription CreateDefault();
	static DepthStencilDescription CreateReadWriteDepth();
	static DepthStencilDescription CreateReadOnlyDepth();
	static DepthStencilDescription CreateNone();

	DepthStencilOperation ClockwiseFace;
	DepthStencilOperation CounterClockwiseFace;
	int32_t ReferenceStencil;
	uint32_t StencilMask;
	uint32_t StencilWriteMask;
	ComparisonFunction DepthBufferFunction;
	bool DepthBufferEnable;
	bool DepthBufferWriteEnable;
	bool StencilEnable;
};

struct DepthStencilFaceOperationGL4
{
	GLenum stencilFunction = GL_ALWAYS;
	GLenum stencilFail = GL_KEEP;
	GLenum stencilDepthBufferFail = GL_KEEP;
	GLenum stencilPass = GL_KEEP;
};

class DepthStencilState
{
public:
	DepthStencilState(const DepthStencilDescription &desc);

	void Bind();

private:
	void applyDepthTest();
	void applyStencilTest();

	DepthStencilFaceOperationGL4 m_clockwiseFace;
	DepthStencilFaceOperationGL4 m_counterClockwiseFace;
	GLenum m_depthFunction;

	GLint m_referenceStencil;
	GLuint m_stencilMask;
	GLuint m_stencilWriteMask;
	GLboolean m_depthBufferWriteEnable;
	bool m_stencilEnable;
	bool m_depthBufferEnable;
};