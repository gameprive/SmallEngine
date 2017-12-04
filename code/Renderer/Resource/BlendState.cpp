// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "BlendState.h"

//-----------------------------------------------------------------------
RenderTargetBlendDescription RenderTargetBlendDescription::CreateAdditive()
{
	RenderTargetBlendDescription desc;
	desc.BlendEnable = true;
	desc.ColorBlendOperation = BlendOperation::Add;
	desc.AlphaBlendOperation = BlendOperation::Add;
	desc.ColorSourceBlend = BlendFactor::SourceAlpha;
	desc.AlphaSourceBlend = BlendFactor::SourceAlpha;
	desc.ColorDestinationBlend = BlendFactor::One;
	desc.AlphaDestinationBlend = BlendFactor::One;
	return desc;
}
//-----------------------------------------------------------------------
RenderTargetBlendDescription RenderTargetBlendDescription::CreateAlphaBlend()
{
	RenderTargetBlendDescription desc;
	desc.BlendEnable = true;
	desc.ColorBlendOperation = BlendOperation::Add;
	desc.AlphaBlendOperation = BlendOperation::Add;
	desc.ColorSourceBlend = BlendFactor::One;
	desc.AlphaSourceBlend = BlendFactor::One;
	desc.ColorDestinationBlend = BlendFactor::InverseSourceAlpha;
	desc.AlphaDestinationBlend = BlendFactor::InverseSourceAlpha;
	return desc;
}
//-----------------------------------------------------------------------
RenderTargetBlendDescription RenderTargetBlendDescription::CreateNonPremultiplied()
{
	RenderTargetBlendDescription desc;
	desc.BlendEnable = true;
	desc.ColorBlendOperation = BlendOperation::Add;
	desc.AlphaBlendOperation = BlendOperation::Add;
	desc.ColorSourceBlend = BlendFactor::SourceAlpha;
	desc.AlphaSourceBlend = BlendFactor::SourceAlpha;
	desc.ColorDestinationBlend = BlendFactor::InverseSourceAlpha;
	desc.AlphaDestinationBlend = BlendFactor::InverseSourceAlpha;
	return desc;
}
//-----------------------------------------------------------------------
RenderTargetBlendDescription RenderTargetBlendDescription::CreateOpaque()
{
	RenderTargetBlendDescription desc;
	desc.BlendEnable = false;
	desc.ColorBlendOperation = BlendOperation::Add;
	desc.AlphaBlendOperation = BlendOperation::Add;
	desc.ColorSourceBlend = BlendFactor::One;
	desc.AlphaSourceBlend = BlendFactor::One;
	desc.ColorDestinationBlend = BlendFactor::Zero;
	desc.AlphaDestinationBlend = BlendFactor::Zero;
	return desc;
}
//-----------------------------------------------------------------------
BlendDescription BlendDescription::CreateDefault()
{
	return CreateOpaque();
}
//-----------------------------------------------------------------------
BlendDescription BlendDescription::CreateAdditive()
{
	BlendDescription desc;
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;
	for ( auto &renderTarget : desc.RenderTargets )
		renderTarget = RenderTargetBlendDescription::CreateAdditive();

	return desc;
}
//-----------------------------------------------------------------------
BlendDescription BlendDescription::CreateAlphaBlend()
{
	BlendDescription desc;
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;
	for ( auto &renderTarget : desc.RenderTargets )
		renderTarget = RenderTargetBlendDescription::CreateAlphaBlend();

	return desc;
}
//-----------------------------------------------------------------------
BlendDescription BlendDescription::CreateNonPremultiplied()
{
	BlendDescription desc;
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;
	for ( auto &renderTarget : desc.RenderTargets )
		renderTarget = RenderTargetBlendDescription::CreateNonPremultiplied();

	return desc;
}
//-----------------------------------------------------------------------
BlendDescription BlendDescription::CreateOpaque()
{
	BlendDescription desc;
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;
	for ( auto &renderTarget : desc.RenderTargets )
		renderTarget = RenderTargetBlendDescription::CreateOpaque();

	return desc;
}
//-----------------------------------------------------------------------
GLenum toBlendGL4(BlendFactor blend)
{
	switch ( blend )
	{
	case BlendFactor::Zero: return GL_ZERO;
	case BlendFactor::One: return GL_ONE;
	case BlendFactor::SourceColor: return GL_SRC_COLOR;
	case BlendFactor::InverseSourceColor: return GL_ONE_MINUS_SRC_COLOR;
	case BlendFactor::SourceAlpha: return GL_SRC_ALPHA;
	case BlendFactor::InverseSourceAlpha: return GL_ONE_MINUS_SRC_ALPHA;
	case BlendFactor::DestinationAlpha: return GL_DST_ALPHA;
	case BlendFactor::InverseDestinationAlpha: return GL_ONE_MINUS_DST_ALPHA;
	case BlendFactor::DestinationColor: return GL_DST_COLOR;
	case BlendFactor::InverseDestinationColor: return GL_ONE_MINUS_DST_COLOR;
	case BlendFactor::SourceAlphaSaturation: return GL_SRC_ALPHA_SATURATE;
	case BlendFactor::BlendFactor: return GL_CONSTANT_COLOR;
	case BlendFactor::InvereseBlendFactor: return GL_ONE_MINUS_CONSTANT_COLOR;
	case BlendFactor::Source1Color: return GL_SRC1_COLOR;
	case BlendFactor::InverseSource1Color: return GL_ONE_MINUS_SRC1_COLOR;
	case BlendFactor::Source1Alpha: return GL_SRC1_ALPHA;
	case BlendFactor::InverseSource1Alpha: return GL_ONE_MINUS_SRC1_ALPHA;
	}
	ParameterFailed("toBlendGL4");
}
//-----------------------------------------------------------------------
GLenum toBlendOperationGL4(BlendOperation operation)
{
	switch ( operation )
	{
	case BlendOperation::Add: return GL_FUNC_ADD;
	case BlendOperation::Subtract: return GL_FUNC_SUBTRACT;
	case BlendOperation::ReverseSubtract: return GL_FUNC_REVERSE_SUBTRACT;
	case BlendOperation::Min: return GL_MIN;
	case BlendOperation::Max: return GL_MAX;
	}
	ParameterFailed("toBlendOperationGL4");
}

//-----------------------------------------------------------------------
BlendState::BlendState(const BlendDescription &desc)
	: m_independentBlendEnable(desc.IndependentBlendEnable)
	, m_alphaToCoverageEnable(desc.AlphaToCoverageEnable)
{
	for ( size_t i = 0; i < 8; ++i )
	{
		auto &curRT = m_renderTargets[i];
		const auto &curRTDesc = desc.RenderTargets[i];

		curRT.ColorSource = toBlendGL4(curRTDesc.ColorSourceBlend);
		curRT.ColorDestination = toBlendGL4(curRTDesc.ColorDestinationBlend);
		curRT.ColorOperation = toBlendOperationGL4(curRTDesc.ColorBlendOperation);
		curRT.AlphaSource = toBlendGL4(curRTDesc.AlphaSourceBlend);
		curRT.AlphaDestination = toBlendGL4(curRTDesc.AlphaDestinationBlend);
		curRT.AlphaOperation = toBlendOperationGL4(curRTDesc.AlphaBlendOperation);
		curRT.BlendEnable = curRTDesc.BlendEnable;
	}
}
//-----------------------------------------------------------------------
void BlendState::Bind()
{
	if ( m_independentBlendEnable )
	{
		GLuint index = 0;
		for ( auto &rt : m_renderTargets )
		{
			if ( rt.BlendEnable )
				glEnablei(GL_BLEND, index);
			else
				glDisablei(GL_BLEND, index);

			glBlendFuncSeparatei(index, rt.ColorSource, rt.ColorDestination, rt.AlphaSource, rt.AlphaDestination);
			glBlendEquationSeparatei(index, rt.ColorOperation, rt.AlphaOperation);

			++index;
		}
	}
	else
	{
		auto &rt = m_renderTargets[0];
		if ( rt.BlendEnable )
			glEnable(GL_BLEND);
		else
			glDisable(GL_BLEND);

		glBlendEquationSeparate(rt.ColorOperation, rt.AlphaOperation);
		glBlendFuncSeparate(rt.ColorSource, rt.ColorDestination, rt.AlphaSource, rt.AlphaDestination);
	}

	if ( m_alphaToCoverageEnable )
		glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
	else
		glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
}
//-----------------------------------------------------------------------