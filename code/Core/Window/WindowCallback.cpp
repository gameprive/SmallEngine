// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "Window.h"
#include "Debug/Log.h"

//--------------------------------------------------------------------
void errorCallback(int /*error*/, const char *description)
{
	Log(LevelLog::Error) << description;
}
//--------------------------------------------------------------------
void windowSizeCallback(GLFWwindow*, int width, int height)
{
	Window::Get().resize(width, height);
}
//--------------------------------------------------------------------
void SetPreCreateCallback()
{
	glfwSetErrorCallback(errorCallback);
}
//--------------------------------------------------------------------
void SetPostCreateCallback(GLFWwindow *window)
{
	glfwSetWindowSizeCallback(window, windowSizeCallback);
}
//--------------------------------------------------------------------
