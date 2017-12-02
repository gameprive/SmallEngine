#pragma once

enum class BlendFactor : uint8_t
{
	Zero,
	One,

	DestinationColor,
	SourceColor,
	InverseDestinationColor,
	InverseSourceColor,

	DestinationAlpha,
	SourceAlpha,
	InverseDestinationAlpha,
	InverseSourceAlpha,
};

enum class BlendOperation : uint8_t
{
	Add,
	Subtract,
	ReverseSubtract,
	Min,
	Max,
};

struct RenderTargetBlendDescription
{
	static RenderTargetBlendDescription CreateAdditive();
	static RenderTargetBlendDescription CreateAlphaBlend();
	static RenderTargetBlendDescription CreateNonPremultiplied();
	static RenderTargetBlendDescription CreateOpaque();

	BlendFactor ColorSourceBlend;
	BlendFactor ColorDestinationBlend;
	BlendOperation ColorBlendOperation;
	BlendFactor AlphaSourceBlend;
	BlendFactor AlphaDestinationBlend;
	BlendOperation AlphaBlendOperation;
	bool BlendEnable;
};

struct BlendDescription
{
	static BlendDescription CreateDefault();
	static BlendDescription CreateAdditive();
	static BlendDescription CreateAlphaBlend();
	static BlendDescription CreateNonPremultiplied();
	static BlendDescription CreateOpaque();

	RenderTargetBlendDescription RenderTargets[8];
	bool AlphaToCoverageEnable;
	bool IndependentBlendEnable;
};

struct RenderTargetBlendDescGL4
{
	GLenum ColorSource;
	GLenum ColorDestination;
	GLenum ColorOperation;
	GLenum AlphaSource;
	GLenum AlphaDestination;
	GLenum AlphaOperation;
	bool BlendEnable;
};

class BlendState
{
public:
	BlendState(const BlendDescription &desc);

	void Bind();

private:
	RenderTargetBlendDescGL4 m_renderTargets[8];
	bool m_independentBlendEnable;
	bool m_alphaToCoverageEnable;
};