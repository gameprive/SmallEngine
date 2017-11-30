#pragma once

NORETURN
inline void OpenGLParameterFailed(const std::string &typeName)
{
	throw std::invalid_argument("failed to '" + typeName + "' OpenGL parameter");
}