#pragma once

#include "Core/Template/Singleton.h"

typedef struct GLFWwindow GLFWwindow;	// ==> <glfw3.h>

struct WindowConfig
{
	std::string title = "3DEngine";

	uint32_t width = 800;
	uint32_t height = 600;
	uint8_t samples = 0;
	bool srgb = false;
	bool fullscreen = false;
	bool fullWindowSize = false;
	bool resizable = true;
	bool vsync = false;
};

class Window : public Singleton<Window>
{
public:
	bool Init(const WindowConfig &config);
	void Close();
	
	void Swap();

	bool IsQuit();

	// GLFW_CURSOR_DISABLED - скрыть и центрировать курсор
	// GLFW_CURSOR_HIDDEN - скрыть курсор
	// GLFW_CURSOR_NORMAL - нормальный режим
	void SetCursorMode(int mode);
	void SetCursorPosition(float x, float y);

	// sample value 'key'  - GLFW_KEY_E
	bool GetKey(int key);	
	// sample value 'button'  - GLFW_MOUSE_BUTTON_LEFT
	bool GetMouseButton(int button);
	void GetMousePos(double &x, double &y);	
	
	uint32_t GetWidth() const { return m_config.width; }
	uint32_t GetHeight() const { return m_config.height; }

	// биндить std::bind(Func, std::placeholders::_1)
	// или std::bind(Class::Func, this, std::placeholders::_1, std::placeholders::_2)
	std::function<void(int width, int height)> windowsResizeHandler;

private:
	void resize(int w, int h);
	friend void windowSizeCallback(GLFWwindow*, int, int);	// ==> "WindowCallback.cpp"

	WindowConfig m_config;
	GLFWwindow *m_window = nullptr;
};