#pragma once

#include "Core/Base/BaseFunc.h"

enum class LevelLog
{
	Error,
	Warning,
	Info
};

class Log
{
public:
	Log(LevelLog pLevel = LevelLog::Info) : m_level(pLevel) {}
	~Log();

	template <typename T>
	Log& operator<< (T val)
	{
		m_s += ToString(val);
		return *this;
	}

	Log& operator<<(const std::string &val)
	{
		m_s += val;
		return *this;
	}

	Log& operator<<(char *val)
	{
		m_s += val;
		return *this;
	}

	Log& operator<<(const char *val)
	{
		m_s += val;
		return *this;
	}

private:
	Log(const Log&) = delete;
	void operator=(const Log&) = delete;
	void operator=(Log&&) = delete;

	LevelLog m_level = LevelLog::Info;
	std::string m_s;
};

#if _DEBUG && WIN32
#	define Assert(expression) static_cast<void>((!!(expression)) || (_CrtDbgBreak(), _ASSERT(expression), false))
#	define AssertMsg(expression, message) static_cast<void>((!!(expression)) || (1 != _CrtDbgReportW(_CRT_ASSERT, _CRT_WIDE(__FILE__), __LINE__, nullptr, L"%s", message)) || (_CrtDbgBreak(), false))
#else
#	define Assert(expression)
#	define AssertMsg(expression, message)
#endif