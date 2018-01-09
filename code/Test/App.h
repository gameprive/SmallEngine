#pragma once

class Test;
class Test2;

static const bool VSYNC = false;
static const double TARGET_FRAME_INTERVAL = 0.016665 * (VSYNC ? 1.0 : 0.0);
static const float FRAME_TIME_ALPHA = 0.25f;

class App
{
public:
	~App();

	void Start();

private:
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

	Test *m_test = nullptr;
};