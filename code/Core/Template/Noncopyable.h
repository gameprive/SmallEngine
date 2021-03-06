﻿#pragma once

class Noncopyable
{
protected:
	Noncopyable() = default;

	Noncopyable(const Noncopyable&) = delete;
	Noncopyable(const Noncopyable&&) = delete;

	void operator=(const Noncopyable&) = delete;
	void operator=(const Noncopyable&&) = delete;
};