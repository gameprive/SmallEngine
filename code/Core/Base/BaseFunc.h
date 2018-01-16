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

// TODO: в Math
inline int64_t Clamp(int64_t _a, int64_t _min, int64_t _max)
{
	const int64_t min = std::min(_a, _max);
	const int64_t result = std::max(_min, min);
	return result;
}