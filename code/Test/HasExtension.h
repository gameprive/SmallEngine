#pragma once

inline bool HasExtension(const char *glExtension)
{
	static std::vector<std::string> sExtensions;
	if ( sExtensions.empty() )
	{
		int count;
		glGetIntegerv(GL_NUM_EXTENSIONS, &count);
		sExtensions.resize(count);

		for ( int i = 0; i < count; ++i )
		{
			sExtensions[i] = std::string((const char*)glGetStringi(GL_EXTENSIONS, i));
		}
	}

	return std::find(std::begin(sExtensions), std::end(sExtensions), glExtension) != std::end(sExtensions);
}

inline const char* GetGLErrorString(GLenum code)
{
	switch ( code )
	{
	case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
	case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
	case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
	case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
	case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
	case GL_FRAMEBUFFER_UNSUPPORTED: return "GL_FRAMEBUFFER_UNSUPPORTED";
	case GL_FRAMEBUFFER_UNDEFINED: return "GL_FRAMEBUFFER_UNDEFINED";
	default: return "other GL error";
	}
}

inline void ErrorCheck(const char* info)
{
	(void)info;
#if defined _DEBUG || defined DEBUG
	GLenum errorCode = GL_INVALID_ENUM;

	while ( (errorCode = glGetError()) != GL_NO_ERROR )
	{
		Log(LevelLog::Error) << std::string(info) << " caused an OpenGL error: " << GetGLErrorString(errorCode);
	}
#endif
}

inline void ErrorCheckFBO()
{
#if defined _DEBUG || defined DEBUG
	const GLenum errorCode = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if ( errorCode == GL_FRAMEBUFFER_COMPLETE )
		return;
	LogError() << "FBO error: " << GetGLErrorString(errorCode);
#endif
}