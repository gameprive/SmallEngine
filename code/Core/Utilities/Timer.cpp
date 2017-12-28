// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "Timer.h"

//--------------------------------------------------------------------
Timer::Timer()
{
	Reset();
}
//--------------------------------------------------------------------
void Timer::Reset()
{
	m_startTime = std::chrono::high_resolution_clock::now();
}
//--------------------------------------------------------------------
uint64_t Timer::GetMilliseconds() const
{
	const auto newTime = std::chrono::high_resolution_clock::now();
	const std::chrono::duration<double> dur = newTime - m_startTime;
	return std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
}
//--------------------------------------------------------------------
uint64_t Timer::GetMicroseconds() const
{
	const auto newTime = std::chrono::high_resolution_clock::now();
	const std::chrono::duration<double> dur = newTime - m_startTime;
	return std::chrono::duration_cast<std::chrono::microseconds>(dur).count();
}
//--------------------------------------------------------------------
uint64_t Timer::GetStartMs() const
{
	const std::chrono::nanoseconds startTimeNs = m_startTime.time_since_epoch();
	return std::chrono::duration_cast<std::chrono::milliseconds>(startTimeNs).count();
}
//--------------------------------------------------------------------
