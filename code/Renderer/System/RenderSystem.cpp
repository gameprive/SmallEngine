// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "RenderSystem.h"
#include "Core/Window/Window.h"

//-----------------------------------------------------------------------
void RenderSystem::SetRenderTarget2D(std::shared_ptr<RenderTarget2D> rt)
{
	if ( m_rt ) m_rt->Unbind(0);
	m_rt.reset();

	if ( !rt )
	{
		SetViewport(0, 0, Window::Get().GetWidth(), Window::Get().GetHeight());
	}		
	else
	{
		rt->Bind(0);
		GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffers); 
		m_rt = rt;
		SetViewport(0, 0, rt->GetWidth(), rt->GetHeight());
	}
}
//-----------------------------------------------------------------------
void RenderSystem::SetViewport(int x, int y, int w, int h)
{
	glViewport(x, y, w, h);
}
//-----------------------------------------------------------------------
void RenderSystem::Clear(const glm::vec4 &color, float depth, uint8_t stencil)
{
	GLbitfield mask = 0;

	mask = GL_COLOR_BUFFER_BIT;
	glClearColor(color.r, color.g, color.b, color.a);
	
	if ( depth )
	{
		mask |= GL_DEPTH_BUFFER_BIT;
		const auto clamped = std::min(std::max(depth, 0.0f), 1.0f);
		glClearDepthf(clamped);
	}

	if ( stencil )
	{
		mask |= GL_STENCIL_BUFFER_BIT;
		glClearStencil(stencil);
	}
	
	glClear(mask);
}
//-----------------------------------------------------------------------