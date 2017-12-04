#pragma once

class Test;

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
	
	float m_dt = 0.0f;
	float m_currentTime;
	float m_oldTime;
	float m_frameTime = 0.0f;
	float m_renderAccum = 0.0f;
	float m_smoothFrameTime = TARGET_FRAME_INTERVAL;

	Test *m_test = nullptr;
};