// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "SamplerState.h"
#include "System/RenderDevice.h"

//-----------------------------------------------------------------------
struct PairTexFilter
{
	GLenum min;
	GLenum mag;
};
//-----------------------------------------------------------------------
PairTexFilter MapTextureFilter(SamplerFilter filter)
{
	switch ( filter )
	{
	case SamplerFilter::Point: return{ GL_NEAREST, GL_NEAREST };
	case SamplerFilter::Linear: return{ GL_LINEAR, GL_LINEAR };
	case SamplerFilter::Anisotropic: return{ GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR };
	case SamplerFilter::LinearMipPoint: return{ GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR };
	case SamplerFilter::PointMipLinear: return{ GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST };
	case SamplerFilter::MinLinearMagPointMipLinear: return{ GL_LINEAR_MIPMAP_LINEAR, GL_NEAREST };
	case SamplerFilter::MinLinearMagPointMipPoint: return{ GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST };
	case SamplerFilter::MinPointMagLinearMipLinear: return{ GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR };
	case SamplerFilter::MinPointMagLinearMipPoint: return{ GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR };
	}
	ParameterFailed("MapTextureFilter");
}
//-----------------------------------------------------------------------
GLenum toGLAddressMode(AddressMode address)
{
	switch ( address )
	{
	case AddressMode::Wrap: return GL_REPEAT;
	case AddressMode::Mirror: return GL_MIRRORED_REPEAT;
	case AddressMode::Clamp: return GL_CLAMP_TO_EDGE;
	case AddressMode::Border: return GL_CLAMP_TO_BORDER;
	}
	ParameterFailed("TextureAddressMode");
}
//-----------------------------------------------------------------------
SamplerDescription SamplerDescription::CreateDefault()
{
	return CreateLinearClamp();
}
//-----------------------------------------------------------------------
SamplerDescription SamplerDescription::CreateAnisotropicClamp()
{
	SamplerDescription desc;
	desc.Filter = SamplerFilter::Anisotropic;
	desc.AddressU = AddressMode::Clamp;
	desc.AddressV = AddressMode::Clamp;
	desc.AddressW = AddressMode::Clamp;
	desc.MipMapLevelOfDetailBias = 0.0f;
	desc.MaxAnisotropy = 16;
	desc.MinMipLevel = 0.0f;
	desc.MaxMipLevel = 1000.0f;
	return desc;
}
//-----------------------------------------------------------------------
SamplerDescription SamplerDescription::CreateAnisotropicWrap()
{
	SamplerDescription desc;
	desc.Filter = SamplerFilter::Anisotropic;
	desc.AddressU = AddressMode::Wrap;
	desc.AddressV = AddressMode::Wrap;
	desc.AddressW = AddressMode::Wrap;
	desc.MipMapLevelOfDetailBias = 0.0f;
	desc.MaxAnisotropy = 16;
	desc.MinMipLevel = 0.0f;
	desc.MaxMipLevel = 1000.0f;
	return desc;
}
//-----------------------------------------------------------------------
SamplerDescription SamplerDescription::CreateLinearClamp()
{
	SamplerDescription desc;
	desc.Filter = SamplerFilter::Linear;
	desc.AddressU = AddressMode::Clamp;
	desc.AddressV = AddressMode::Clamp;
	desc.AddressW = AddressMode::Clamp;
	desc.MipMapLevelOfDetailBias = 0.0f;
	desc.MaxAnisotropy = 0;
	desc.MinMipLevel = 0.0f;
	desc.MaxMipLevel = std::numeric_limits<float>::max();
	return desc;
}
//-----------------------------------------------------------------------
SamplerDescription SamplerDescription::CreateLinearWrap()
{
	SamplerDescription desc;
	desc.Filter = SamplerFilter::Linear;
	desc.AddressU = AddressMode::Wrap;
	desc.AddressV = AddressMode::Wrap;
	desc.AddressW = AddressMode::Wrap;
	desc.MipMapLevelOfDetailBias = 0.0f;
	desc.MaxAnisotropy = 0;
	desc.MinMipLevel = 0.0f;
	desc.MaxMipLevel = std::numeric_limits<float>::max();
	return desc;
}
//-----------------------------------------------------------------------
SamplerDescription SamplerDescription::CreatePointClamp()
{
	SamplerDescription desc;
	desc.Filter = SamplerFilter::Point;
	desc.AddressU = AddressMode::Clamp;
	desc.AddressV = AddressMode::Clamp;
	desc.AddressW = AddressMode::Clamp;
	desc.MipMapLevelOfDetailBias = 0.0f;
	desc.MaxAnisotropy = 0;
	desc.MinMipLevel = 0.0f;
	desc.MaxMipLevel = std::numeric_limits<float>::max();
	return desc;
}
//-----------------------------------------------------------------------
SamplerDescription SamplerDescription::CreatePointWrap()
{
	SamplerDescription desc;
	desc.Filter = SamplerFilter::Point;
	desc.AddressU = AddressMode::Wrap;
	desc.AddressV = AddressMode::Wrap;
	desc.AddressW = AddressMode::Wrap;
	desc.MipMapLevelOfDetailBias = 0.0f;
	desc.MaxAnisotropy = 0;
	desc.MinMipLevel = 0.0f;
	desc.MaxMipLevel = std::numeric_limits<float>::max();
	return desc;
}
//-----------------------------------------------------------------------
SamplerState::SamplerState(const SamplerDescription &desc)
{
	Assert(desc.MinMipLevel <= desc.MaxMipLevel);
	Assert(desc.MaxMipLevel <= std::numeric_limits<float>::max());

	// filter
	const PairTexFilter filter = MapTextureFilter(desc.Filter);

	// color
	float fcolor[4];
	fcolor[0] = static_cast<float>((desc.BorderColor >> 0) & 0xFF) / 255.0f;
	fcolor[1] = static_cast<float>((desc.BorderColor >> 8) & 0xFF) / 255.0f;
	fcolor[2] = static_cast<float>((desc.BorderColor >> 16) & 0xFF) / 255.0f;
	fcolor[3] = static_cast<float>((desc.BorderColor >> 24) & 0xFF) / 255.0f;

	// init sampler
	glGenSamplers(1, &m_samplerObject);
	glSamplerParameteri(m_samplerObject, GL_TEXTURE_WRAP_S, toGLAddressMode(desc.AddressU));
	glSamplerParameteri(m_samplerObject, GL_TEXTURE_WRAP_T, toGLAddressMode(desc.AddressV));
	glSamplerParameteri(m_samplerObject, GL_TEXTURE_WRAP_R, toGLAddressMode(desc.AddressW));

	glSamplerParameteri(m_samplerObject, GL_TEXTURE_MIN_FILTER, filter.min);
	glSamplerParameteri(m_samplerObject, GL_TEXTURE_MAG_FILTER, filter.mag);
	if ( desc.Filter == SamplerFilter::Anisotropic )
	{
		Assert(1 <= desc.MaxAnisotropy);
		uint32_t deviceMaxAnisotropy = RenderDevice::Get().GetMaxAnisotropy();
		deviceMaxAnisotropy = std::min(deviceMaxAnisotropy, desc.MaxAnisotropy);
		glSamplerParameterf(m_samplerObject, GL_TEXTURE_MAX_ANISOTROPY_EXT, deviceMaxAnisotropy);
	}
	
	glSamplerParameterf(m_samplerObject, GL_TEXTURE_MIN_LOD, desc.MinMipLevel);
	glSamplerParameterf(m_samplerObject, GL_TEXTURE_MAX_LOD, desc.MaxMipLevel);
	glSamplerParameterf(m_samplerObject, GL_TEXTURE_LOD_BIAS, desc.MipMapLevelOfDetailBias);

	glSamplerParameterfv(m_samplerObject, GL_TEXTURE_BORDER_COLOR, fcolor);
}
//-----------------------------------------------------------------------
SamplerState::~SamplerState()
{
	glDeleteSamplers(1, &m_samplerObject);
}
//-----------------------------------------------------------------------
void SamplerState::Bind(uint32_t index)
{
	int i = GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS;
	Assert(index <= GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);
	Assert(m_samplerObject);
	glBindSampler(index, m_samplerObject);
}
//-----------------------------------------------------------------------