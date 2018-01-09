﻿// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "App.h"
#include "Test.h"
#include "Test2.h"

//--------------------------------------------------------------------
App::~App()
{
	close();
}
//--------------------------------------------------------------------
void App::Start()
{
	srand(static_cast<unsigned>(time(nullptr)));

	WindowConfig config;
	config.samples = 1;
	config.width = 1024;
	config.height = 768;
	if ( m_window.Init(config) )
	{
		m_window.windowsResizeHandler = std::bind(&App::resize, this, std::placeholders::_1, std::placeholders::_2);

		m_renderDevice.Init();

		prepareOpenGL();

		m_test = new Test(&m_window);
		m_currentTime = glfwGetTime();
		while ( !m_window.GetKey(GLFW_KEY_ESCAPE) && frame() );
	}
	close();
}
//--------------------------------------------------------------------
void App::prepareOpenGL()
{
	glClearColor(0.3f, 0.8f, 1.0f, 1.0f);
}
//--------------------------------------------------------------------
bool App::frame()
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
				
		m_test->Update(m_smoothFrameTime);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_test->Render();
	m_window.Swap();

	return !m_window.IsQuit();
}
//--------------------------------------------------------------------
void App::close()
{
	delete m_test; m_test = nullptr;
	m_window.Close();
}
//--------------------------------------------------------------------
void App::resize(int width, int height)
{
	glViewport(0, 0, width, height);
	m_test->Resize(width, height);
}
//--------------------------------------------------------------------