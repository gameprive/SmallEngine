#pragma once

#include "Noncopyable.h"

template<typename T>
class Singleton : Noncopyable
{
public:
	Singleton()
	{
		m_type = static_cast<T*>(this);
	}
	~Singleton()
	{
		m_type = nullptr;
	}
	static T& Get()
	{
		assert(m_type);
		return (*m_type);
	}
	static T* GetPtr()
	{
		assert(m_type);
		return m_type;
	}

private:
	static T *m_type;
};

template<typename T> T *Singleton<T>::m_type = nullptr;