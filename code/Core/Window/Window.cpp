// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "Window.h"
#include "Debug/Log.h"

//--------------------------------------------------------------------
TODO("нужно как-то определять какую версию создавать");
uint16_t apiOGLMajorVersion = 4;
uint16_t apiOGLMinorVersion = 3;
//--------------------------------------------------------------------
void SetPreCreateCallback();						// ==> "WindowCallback.cpp"
void SetPostCreateCallback(GLFWwindow *window);		// ==> "WindowCallback.cpp"
//--------------------------------------------------------------------
// https://blog.nobel-joergensen.com/2013/02/17/debugging-opengl-part-2-using-gldebugmessagecallback/
// http://steps3d.narod.ru/tutorials/debug-output-tutorial.html
//--------------------------------------------------------------------
#ifdef WIN32
static void APIENTRY openglErrorCallback(
#else
static void openglErrorCallback(
#endif
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei /*length*/,
	const GLchar* message,
	const void* /*userParam*/)
{
	LogError log;
	log << "OpenGL: " << message << " [";

	log << "source: ";
	switch ( source )
	{
	case GL_DEBUG_SOURCE_API: log << "API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM: log << "System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: log << "ShaderCompiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY: log << "ThirdParty"; break;
	case GL_DEBUG_SOURCE_APPLICATION: log << "Application"; break;
	case GL_DEBUG_SOURCE_OTHER: log << "Other"; break;
	}

	log << " type: ";
	switch ( type )
	{
	case GL_DEBUG_TYPE_ERROR: log << "Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: log << "Deprecated"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: log << "UndefinedBehavior"; break;
	case GL_DEBUG_TYPE_PORTABILITY: log << "Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE: log << "Performance"; break;
	case GL_DEBUG_TYPE_OTHER: log << "Other"; break;

	case GL_DEBUG_TYPE_MARKER: log << "Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP: log << "PushGroup"; break;
	case GL_DEBUG_TYPE_POP_GROUP: log << "PopGroup"; break;
	}
	log << " id: " << id;
	log << " severity: ";
	switch ( severity )
	{
	case GL_DEBUG_SEVERITY_LOW: log << "Low"; break;
	case GL_DEBUG_SEVERITY_MEDIUM: log << "Medium"; break;
	case GL_DEBUG_SEVERITY_HIGH: log << "High"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: log << "Notification"; break;
	}
	log << "]";
}
//--------------------------------------------------------------------
void InitGLDebugger()
{
	GLint v;
	glGetIntegerv(GL_CONTEXT_FLAGS, &v);
	if ( (v & GL_CONTEXT_FLAG_DEBUG_BIT) && glDebugMessageCallback )
	{		
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(openglErrorCallback, nullptr);
		
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

		glDebugMessageControl(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_MARKER, GL_DONT_CARE, 0, nullptr, GL_FALSE);
		glDebugMessageControl(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_PUSH_GROUP, GL_DONT_CARE, 0, nullptr, GL_FALSE);
		glDebugMessageControl(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_POP_GROUP, GL_DONT_CARE, 0, nullptr, GL_FALSE);

		Log(LevelLog::Info) << "OpenGL debug output extension enabled";
	}
	else
		Log(LevelLog::Info) << "OpenGL debug output extension not available";
}
//--------------------------------------------------------------------
bool Window::Init(const WindowConfig &config)
{
	m_config = config;

	//==========================================================
	// Init GLFW system and Window
	//==========================================================
	SetPreCreateCallback();
	if ( !glfwInit() )
	{
		Log(LevelLog::Error) << "Failed to initialize GLFW";
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, apiOGLMajorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, apiOGLMinorVersion);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_REFRESH_RATE, 60);
	glfwWindowHint(GLFW_RESIZABLE, m_config.resizable);
#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif
	glfwWindowHint(GLFW_SAMPLES, m_config.samples);
	glfwWindowHint(GLFW_SRGB_CAPABLE, m_config.srgb ? GL_TRUE : GL_FALSE);

	GLFWmonitor *monitor = nullptr;
	int windowWidth = m_config.width;
	int windowHeight = m_config.height;
	if ( m_config.fullscreen || m_config.fullWindowSize )
	{
		const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		windowWidth = mode->width;
		windowHeight = mode->height;

		if ( m_config.fullscreen ) monitor = glfwGetPrimaryMonitor();
	}
	m_window = glfwCreateWindow(windowWidth, windowHeight, m_config.title.c_str(), monitor, nullptr);
	if ( !m_window )
	{
		Log(LevelLog::Error) << "Failed to open GLFW window";
		return false;
	}
	TODO("glfwSetWindowIcon");
	glfwMakeContextCurrent(m_window);
	glfwShowWindow(m_window);
	
	TODO("glfwSetInputMode????");
	glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);
	
	SetPostCreateCallback(m_window);
	
	glfwSwapInterval(m_config.vsync ? 1 : 0);
	
	//==========================================================
	// Init GLEW
	//==========================================================
	glewExperimental = true;
	const GLenum error = glewInit();
	if ( error != GLEW_OK )
	{
		Log(LevelLog::Error) << reinterpret_cast<const char*>(glewGetErrorString(error));
		return false;
	}

#ifdef _DEBUG
	InitGLDebugger();
#endif
	
	//==========================================================
	//  PreResize
	//==========================================================
	resize(windowWidth, windowHeight);
	glViewport(0, 0, windowWidth, windowHeight);

	//==========================================================
	//  Set SRGB and Multisample
	//==========================================================
	{
		Log infoLog(LevelLog::Info);
		int res;
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING, &res);
		if ( res == GL_LINEAR || !m_config.srgb )
			infoLog << ":\n\tLinear RGB Default Framebuffer.";
		else if ( res == GL_SRGB )
		{
			infoLog << ":\n\tsRGB Default Framebuffer.";
			glEnable(GL_FRAMEBUFFER_SRGB);
		}

		glGetIntegerv(GL_SAMPLES, &res);
		if ( res > 0 )
		{
			infoLog << "\n\tMultisampled Default Framebuffer. Samples: " << res;
			glEnable(GL_MULTISAMPLE);
		}
		else
			infoLog << "\n\tNon-Multisampled Default Framebuffer.";

		infoLog << "\n";
	}
				
	return true;
}
//--------------------------------------------------------------------
void Window::Close()
{
	if ( m_window )
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
		m_window = nullptr;
	}
}
//--------------------------------------------------------------------
void Window::Swap()
{
	glfwSwapBuffers(m_window);
	glfwPollEvents();
}
//--------------------------------------------------------------------
bool Window::IsQuit()
{
	return (glfwWindowShouldClose(m_window) != 0);
}
//--------------------------------------------------------------------
void Window::SetCursorMode(int mode)
{
	glfwSetInputMode(m_window, GLFW_CURSOR, mode);
}
//--------------------------------------------------------------------
void Window::SetCursorPosition(float x, float y)
{
	glfwSetCursorPos(m_window, x, y);
}
//--------------------------------------------------------------------
bool Window::GetKey(int key)
{
	if ( glfwGetKey(m_window, key) == GLFW_PRESS ) return true;
	return false;
}
//--------------------------------------------------------------------
bool Window::GetMouseButton(int button)
{
	if ( glfwGetMouseButton(m_window, button) == GLFW_PRESS ) return true;
	return false;
}
//--------------------------------------------------------------------
void Window::GetMousePos(double &x, double &y)
{
	glfwGetCursorPos(m_window, &x, &y);
}
//--------------------------------------------------------------------
void Window::resize(int w, int h)
{
	m_config.width = w;
	m_config.height = h;
	if ( windowsResizeHandler ) windowsResizeHandler(w, h);
}
//--------------------------------------------------------------------
