#pragma once

#include "Core/Base/BaseFunc.h"
#include "Core/Template/Noncopyable.h"

enum class LevelLog
{
	Error,
	Warning,
	Info
};

class Log : Noncopyable
{
public:
	Log(LevelLog pLevel = LevelLog::Info) : m_level(pLevel) {}
	~Log();

	template <typename T>
	Log& operator<< (T val)
	{
		m_text += ToString(val);
		return *this;
	}

	Log& operator<<(const std::string &val)
	{
		m_text += val;
		return *this;
	}

	Log& operator<<(char *val)
	{
		m_text += val;
		return *this;
	}

	Log& operator<<(const char *val)
	{
		m_text += val;
		return *this;
	}

private:
	LevelLog m_level;
	std::string m_text;
};

#if _DEBUG && WIN32
#	define Assert(expression) static_cast<void>((!!(expression)) || (_CrtDbgBreak(), _ASSERT(expression), false))
#	define AssertMsg(expression, message) static_cast<void>((!!(expression)) || (1 != _CrtDbgReportW(_CRT_ASSERT, _CRT_WIDE(__FILE__), __LINE__, nullptr, L"%s", message)) || (_CrtDbgBreak(), false))
#else
#	define Assert(expression)
#	define AssertMsg(expression, message)
#endif