#pragma once

#include "Core/Base/BaseFunc.h"
#include "Core/Template/Noncopyable.h"

enum class LevelLog
{
	Error,
	Warning,
	Info, 
	Debug
};

class Log : Noncopyable
{
public:
	Log(LevelLog level = LevelLog::Info) : m_level(level) {}
	~Log();

	void SetLevel(LevelLog level)
	{
		m_level = level;
	}

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

class LogError : public Log
{
public:
	LogError() : Log(LevelLog::Error) {}
};

#if _DEBUG && WIN32
#	define Assert(expression) static_cast<void>((!!(expression)) || (_CrtDbgBreak(), _ASSERT(expression), false))
#else
#	define Assert(expression)
#endif