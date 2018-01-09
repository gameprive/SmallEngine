#pragma once

static const bool VSYNC = false;
static const double TARGET_FRAME_INTERVAL = 0.016665 * (VSYNC ? 1.0 : 0.0);
static const float FRAME_TIME_ALPHA = 0.25f;

#include "App.h"

class Engine
{
public:
	template <typename T>
	void Start()
	{
		m_app = std::make_unique<T>();
		if ( !m_app ) return;
		
		start();
		close();
	}	

private:
	void start();
	void prepareOpenGL();
	bool frame();
	void close();
	void resize(int width, int height);

	Window m_window;
	RenderDevice m_renderDevice;
	RenderSystem m_system;

	double m_dt = 0.0;
	double m_currentTime;
	double m_oldTime;
	double m_frameTime = 0.0;
	double m_renderAccum = 0.0;
	double m_smoothFrameTime = TARGET_FRAME_INTERVAL;

	std::unique_ptr<IApp> m_app;
};