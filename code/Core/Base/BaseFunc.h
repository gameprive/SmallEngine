#pragma once

template<class T>
inline std::string ToString(T val)
{
	return std::to_string(val);
}

NORETURN inline void ParameterFailed(const std::string &typeName)
{
	throw std::invalid_argument("failed to '" + typeName + "' parameter");
}