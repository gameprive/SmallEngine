#pragma once

enum class FillMode : uint8_t
{
	WireFrame,
	Solid,
};

enum class CullingMode : uint8_t
{
	None,
	ClockwiseFace,
	CounterClockwiseFace,
};

struct RasterizerDescription
{
	static RasterizerDescription CreateDefault();
	static RasterizerDescription CreateCullClockwise();
	static RasterizerDescription CreateCullCounterClockwise();
	static RasterizerDescription CreateCullNone();

	float DepthBias;
	float SlopeScaledDepthBias;
	CullingMode CullingMode;
	FillMode FillMode;
	bool MultisampleEnable;
	bool ScissorTestEnable;
};

class RasterizerState
{
public:
	RasterizerState(const RasterizerDescription &desc);

	void Bind();

private:
	GLfloat m_depthBias;
	GLfloat m_slopeScaledDepthBias;
	CullingMode m_cullMode;
	GLenum m_fillMode;
	bool m_multisampleAntiAliasEnable;
	bool m_scissorTestEnable;
};