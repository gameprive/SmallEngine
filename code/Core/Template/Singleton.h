#pragma once

#include "Noncopyable.h"

template<typename Implementation>
class Singleton : Noncopyable
{
public:
	Singleton()
	{
		assert(!m_type);
		m_type = static_cast<Implementation*>(this);
	}
	~Singleton()
	{
		m_type = nullptr;
	}
	static Implementation& Get()
	{
		assert(m_type);
		return (*m_type);
	}
	static Implementation* GetPtr()
	{
		assert(m_type);
		return m_type;
	}

private:
	static Implementation *m_type;
};

template<typename Implementation> Implementation *Singleton<Implementation>::m_type = nullptr;