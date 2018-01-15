// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "Engine.h"

//--------------------------------------------------------------------
void Engine::start(const EngineConfig &config)
{
	if ( m_window.Init(config.window) )
	{
		m_window.windowsResizeHandler = std::bind(&Engine::resize, this, std::placeholders::_1, std::placeholders::_2);

		m_renderDevice.Init();

		if ( m_app->Init() )
		{
			m_currentTime = glfwGetTime();
			while ( !m_window.GetKey(GLFW_KEY_ESCAPE) && frame() );
		}		
	}
}
//--------------------------------------------------------------------
bool Engine::frame()
{
	m_oldTime = m_currentTime;
	m_currentTime = glfwGetTime();
	m_dt = m_currentTime - m_oldTime;
	m_renderAccum += m_dt;
	m_frameTime += m_dt;

	if ( m_renderAccum >= TARGET_FRAME_INTERVAL )
	{
		m_renderAccum -= TARGET_FRAME_INTERVAL;
		m_smoothFrameTime = (m_smoothFrameTime * FRAME_TIME_ALPHA) + (m_frameTime * (1.0 - FRAME_TIME_ALPHA));
		m_frameTime = 0.0f;

		m_app->Update(m_smoothFrameTime);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_app->Render();
	m_window.Swap();

	return !m_window.IsQuit();
}
//--------------------------------------------------------------------
void Engine::close()
{
	if ( m_app ) m_app->Close();
	m_app.release();
	m_window.Close();
}
//--------------------------------------------------------------------
void Engine::resize(int width, int height)
{
	glViewport(0, 0, width, height);
}
//--------------------------------------------------------------------