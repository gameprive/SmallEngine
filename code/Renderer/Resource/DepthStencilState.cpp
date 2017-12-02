// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "DepthStencilState.h"

//-----------------------------------------------------------------------
DepthStencilDescription DepthStencilDescription::CreateDefault()
{
	return CreateReadWriteDepth();
}
//-----------------------------------------------------------------------
DepthStencilDescription DepthStencilDescription::CreateReadWriteDepth()
{
	DepthStencilDescription desc;
	desc.DepthBufferEnable = true;
	desc.DepthBufferWriteEnable = true;
	desc.StencilEnable = false;
	desc.DepthBufferFunction = ComparisonFunction::LessEqual;
	desc.ReferenceStencil = 0;
	desc.StencilMask = std::numeric_limits<uint32_t>::max();
	desc.StencilWriteMask = std::numeric_limits<uint32_t>::max();
	DepthStencilOperation defaultOperation =
	{
		StencilOperation::Keep, StencilOperation::Keep,
		StencilOperation::Keep, ComparisonFunction::Always
	};
	desc.ClockwiseFace = defaultOperation;
	desc.CounterClockwiseFace = defaultOperation;
	return desc;
}
//-----------------------------------------------------------------------
DepthStencilDescription DepthStencilDescription::CreateReadOnlyDepth()
{
	DepthStencilDescription desc;
	desc.DepthBufferEnable = true;
	desc.DepthBufferWriteEnable = false;
	desc.StencilEnable = false;
	desc.DepthBufferFunction = ComparisonFunction::LessEqual;
	desc.ReferenceStencil = 0;
	desc.StencilMask = std::numeric_limits<uint32_t>::max();
	desc.StencilWriteMask = std::numeric_limits<uint32_t>::max();
	DepthStencilOperation defaultOperation =
	{
		StencilOperation::Keep, StencilOperation::Keep,
		StencilOperation::Keep, ComparisonFunction::Always
	};
	desc.ClockwiseFace = defaultOperation;
	desc.CounterClockwiseFace = defaultOperation;
	return desc;
}
//-----------------------------------------------------------------------
DepthStencilDescription DepthStencilDescription::CreateNone()
{
	DepthStencilDescription desc;
	desc.DepthBufferEnable = false;
	desc.DepthBufferWriteEnable = false;
	desc.StencilEnable = false;
	desc.DepthBufferFunction = ComparisonFunction::Always;
	desc.ReferenceStencil = 0;
	desc.StencilMask = std::numeric_limits<uint32_t>::max();
	desc.StencilWriteMask = std::numeric_limits<uint32_t>::max();
	DepthStencilOperation defaultOperation =
	{
		StencilOperation::Keep, StencilOperation::Keep,
		StencilOperation::Keep, ComparisonFunction::Always
	};
	desc.ClockwiseFace = defaultOperation;
	desc.CounterClockwiseFace = defaultOperation;
	return desc;
}
//-----------------------------------------------------------------------
GLenum toComparisonFunctionGL4(ComparisonFunction comparison)
{
	switch ( comparison )
	{
	case ComparisonFunction::Always: return GL_ALWAYS;
	case ComparisonFunction::Equal: return GL_EQUAL;
	case ComparisonFunction::Greater: return GL_GREATER;
	case ComparisonFunction::GreaterEqual: return GL_GEQUAL;
	case ComparisonFunction::Less: return GL_LESS;
	case ComparisonFunction::LessEqual: return GL_LEQUAL;
	case ComparisonFunction::Never: return GL_NEVER;
	case ComparisonFunction::NotEqual: return GL_NOTEQUAL;
	}
	ParameterFailed("toComparisonFunctionGL4");
}
//-----------------------------------------------------------------------
GLenum toStencilOperationGL4(StencilOperation operation)
{
	switch ( operation )
	{
	case StencilOperation::Decrement: return GL_DECR_WRAP;
	case StencilOperation::DecrementSaturation: return GL_DECR;
	case StencilOperation::Increment: return GL_INCR_WRAP;
	case StencilOperation::IncrementSaturation: return GL_INCR;
	case StencilOperation::Invert: return GL_INVERT;
	case StencilOperation::Keep: return GL_KEEP;
	case StencilOperation::Replace: return GL_REPLACE;
	case StencilOperation::Zero: return GL_ZERO;
	}
	ParameterFailed("toStencilOperationGL4");
}
//-----------------------------------------------------------------------
void toDepthStencilFaceOperationGL4(const DepthStencilOperation &face, DepthStencilFaceOperationGL4 &result)
{
	result.stencilFunction = toComparisonFunctionGL4(face.StencilFunction);
	result.stencilDepthBufferFail = toStencilOperationGL4(face.StencilDepthBufferFail);
	result.stencilFail = toStencilOperationGL4(face.StencilFail);
	result.stencilPass = toStencilOperationGL4(face.StencilPass);
}
//-----------------------------------------------------------------------
DepthStencilState::DepthStencilState(const DepthStencilDescription &desc)
	: m_depthFunction(toComparisonFunctionGL4(desc.DepthBufferFunction))
	, m_referenceStencil(desc.ReferenceStencil)
	, m_stencilMask(desc.StencilMask)
	, m_stencilWriteMask(desc.StencilWriteMask)
	, m_depthBufferWriteEnable(desc.DepthBufferWriteEnable ? GL_TRUE : GL_FALSE)
	, m_stencilEnable(desc.StencilEnable)
	, m_depthBufferEnable(desc.DepthBufferEnable)
{
	toDepthStencilFaceOperationGL4(desc.ClockwiseFace, m_clockwiseFace);
	toDepthStencilFaceOperationGL4(desc.CounterClockwiseFace, m_counterClockwiseFace);
}
//-----------------------------------------------------------------------
void DepthStencilState::Bind()
{
	applyDepthTest();
	applyStencilTest();
}
//-----------------------------------------------------------------------
void DepthStencilState::applyDepthTest()
{
	if ( !m_depthBufferEnable )
	{
		glDisable(GL_DEPTH_TEST);
		return;
	}
	glEnable(GL_DEPTH_TEST);

	Assert(m_depthBufferWriteEnable == GL_TRUE || m_depthBufferWriteEnable == GL_FALSE);
	glDepthMask(m_depthBufferWriteEnable);

	glDepthFunc(m_depthFunction);
}
//-----------------------------------------------------------------------
void DepthStencilState::applyStencilTest()
{
	if ( !m_stencilEnable )
	{
		glDisable(GL_STENCIL_TEST);
		return;
	}

	glEnable(GL_STENCIL_TEST);

	// ClockwiseFace:
	glStencilFuncSeparate(GL_FRONT,
						  m_clockwiseFace.stencilFunction,
						  m_referenceStencil,
						  m_stencilMask);

	glStencilOpSeparate(GL_FRONT,
						m_clockwiseFace.stencilFail,
						m_clockwiseFace.stencilDepthBufferFail,
						m_clockwiseFace.stencilPass);

	// CounterClockwiseFace:
	glStencilFuncSeparate(GL_BACK,
						  m_counterClockwiseFace.stencilFunction,
						  m_referenceStencil,
						  m_stencilMask);

	glStencilOpSeparate(GL_BACK,
						m_counterClockwiseFace.stencilFail,
						m_counterClockwiseFace.stencilDepthBufferFail,
						m_counterClockwiseFace.stencilPass);

	glStencilMask(m_stencilWriteMask);
}
//-----------------------------------------------------------------------