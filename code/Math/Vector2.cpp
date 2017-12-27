﻿// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Mathf.h"

//-----------------------------------------------------------------------
Vector2::Vector2(const Vector3 &v3) :
	x(v3.x),
	y(v3.y)
{
}
//-----------------------------------------------------------------------
Vector2 Vector2::operator *(float value) const
{
	return Vector2(x * value, y * value);
}
//-----------------------------------------------------------------------
Vector2& Vector2::operator *=(float value)
{
	x *= value;
	y *= value;
	return *this;
}
//-----------------------------------------------------------------------
Vector2 Vector2::operator +(const Vector2 &value) const
{
	return Vector2(x + value.x, y + value.y);
}
//-----------------------------------------------------------------------
Vector2 Vector2::operator -(const Vector2 &value) const
{
	return Vector2(x - value.x, y - value.y);
}
//-----------------------------------------------------------------------
float Vector2::Magnitude() const
{
	return sqrt(SqrMagnitude());
}
//-----------------------------------------------------------------------
float Vector2::SqrMagnitude() const
{
	return x * x + y * y;
}
//-----------------------------------------------------------------------
bool Vector2::operator ==(const Vector2 &value) const
{
	return Mathf::FloatEqual(x, value.x) && Mathf::FloatEqual(y, value.y);
}
//-----------------------------------------------------------------------
bool Vector2::operator !=(const Vector2 &value) const
{
	return !(*this == value);
}
//-----------------------------------------------------------------------
std::string Vector2::ToString() const
{
	return '(' + std::to_string(x) + ',' + std::to_string(y) + ')';
}
//-----------------------------------------------------------------------