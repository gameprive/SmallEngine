#pragma once

enum class SamplerFilter : uint8_t
{
	Point,
	Linear,
	Anisotropic,
	LinearMipPoint,
	PointMipLinear,
	MinLinearMagPointMipLinear,
	MinLinearMagPointMipPoint,
	MinPointMagLinearMipLinear,
	MinPointMagLinearMipPoint,
};

enum class AddressMode : uint8_t
{
	Wrap,
	Mirror,
	Clamp,
	Border,
};

struct SamplerDescription
{
	static SamplerDescription CreateDefault();
	static SamplerDescription CreateAnisotropicClamp();
	static SamplerDescription CreateAnisotropicWrap();
	static SamplerDescription CreateLinearClamp();
	static SamplerDescription CreateLinearWrap();
	static SamplerDescription CreatePointClamp();
	static SamplerDescription CreatePointWrap();

	SamplerFilter Filter = SamplerFilter::Linear;
	AddressMode AddressU = AddressMode::Clamp;
	AddressMode AddressV = AddressMode::Clamp;
	AddressMode AddressW = AddressMode::Clamp;

	uint32_t BorderColor = 0xFFFFFFFF;
	
	uint32_t MaxAnisotropy = 0;	
	float MinMipLevel = -1000.0f;
	float MaxMipLevel = 1000.0f;
	float MipMapLevelOfDetailBias = 0.0f;
};

class SamplerState : Noncopyable
{
public:
	SamplerState(const SamplerDescription &desc);
	~SamplerState();

	void Bind(uint32_t index);

private:
	GLuint m_samplerObject = 0;
};