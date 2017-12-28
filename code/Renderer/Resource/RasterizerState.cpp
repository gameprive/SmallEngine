// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "RasterizerState.h"

//--------------------------------------------------------------------
RasterizerDescription RasterizerDescription::CreateDefault()
{
	return CreateCullCounterClockwise();
}
//--------------------------------------------------------------------
RasterizerDescription RasterizerDescription::CreateCullClockwise()
{
	RasterizerDescription desc;
	desc.CullingMode = CullingMode::ClockwiseFace;
	desc.FillMode = FillMode::Solid;
	desc.DepthBias = 0.0f;
	desc.SlopeScaledDepthBias = 0.0f;
	desc.MultisampleEnable = false;
	desc.ScissorTestEnable = false;
	return desc;
}
//--------------------------------------------------------------------
RasterizerDescription RasterizerDescription::CreateCullCounterClockwise()
{
	RasterizerDescription desc;
	desc.CullingMode = CullingMode::CounterClockwiseFace;
	desc.FillMode = FillMode::Solid;
	desc.DepthBias = 0.0f;
	desc.SlopeScaledDepthBias = 0.0f;
	desc.MultisampleEnable = false;
	desc.ScissorTestEnable = false;
	return desc;
}
//--------------------------------------------------------------------
RasterizerDescription RasterizerDescription::CreateCullNone()
{
	RasterizerDescription desc;
	desc.CullingMode = CullingMode::None;
	desc.FillMode = FillMode::Solid;
	desc.DepthBias = 0.0f;
	desc.SlopeScaledDepthBias = 0.0f;
	desc.MultisampleEnable = false;
	desc.ScissorTestEnable = false;
	return desc;
}
//--------------------------------------------------------------------
GLenum toFillModeGL4(const FillMode &fillMode)
{
	switch ( fillMode )
	{
	case FillMode::Solid: return  GL_FILL;
	case FillMode::WireFrame: return  GL_LINE;
	}
	return GL_FILL;
}
//--------------------------------------------------------------------
RasterizerState::RasterizerState(const RasterizerDescription &desc)
	: m_fillMode(toFillModeGL4(desc.FillMode))
	, m_cullMode(desc.CullingMode)
	, m_depthBias(desc.DepthBias)
	, m_slopeScaledDepthBias(desc.SlopeScaledDepthBias)
	, m_multisampleAntiAliasEnable(desc.MultisampleEnable)
	, m_scissorTestEnable(desc.ScissorTestEnable)
{
}
//--------------------------------------------------------------------
void RasterizerState::Bind()
{
	// CullingMode:
	switch ( m_cullMode )
	{
	case CullingMode::None:
		glDisable(GL_CULL_FACE);
		break;
	case CullingMode::ClockwiseFace:
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		break;
	case CullingMode::CounterClockwiseFace:
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		break;
	};

	// FillMode:
	glPolygonMode(GL_FRONT_AND_BACK, m_fillMode);

	// Scissor Test:
	if ( m_scissorTestEnable )
		glEnable(GL_SCISSOR_TEST);
	else
		glDisable(GL_SCISSOR_TEST);

	// Depth bias:
	if ( m_depthBias != 0 && m_slopeScaledDepthBias != 0 )
	{
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(m_slopeScaledDepthBias, m_depthBias);
	}
	else
		glDisable(GL_POLYGON_OFFSET_FILL);

	// Multisample Anti-Aliasing:
	if ( m_multisampleAntiAliasEnable )
		glEnable(GL_MULTISAMPLE);
	else
		glDisable(GL_MULTISAMPLE);
}
//--------------------------------------------------------------------
