﻿// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Mathf.h"

//-----------------------------------------------------------------------
const Vector3 Vector3::Zero = Vector3(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::One = Vector3(1.0f, 1.0f, 1.0f);
//-----------------------------------------------------------------------
Vector3::Vector3(float x, float y, float z) :
	x(x), y(y), z(z)
{
}
//-----------------------------------------------------------------------
Vector3::Vector3(const Vector2 &v2) :
	x(v2.x),
	y(v2.y),
	z(0.0f)
{
}
//-----------------------------------------------------------------------
void Vector3::Normalize()
{
	const float sqr_magnitude = SqrMagnitude();
	if ( !Mathf::FloatEqual(sqr_magnitude, 0) )
	{
		float sq = sqrt(sqr_magnitude);

		float inv = 1.0f / sq;
		x = x * inv;
		y = y * inv;
		z = z * inv;
	}
}
//-----------------------------------------------------------------------
Vector3 Vector3::Normalize(const Vector3 &value)
{
	Vector3 v = value;
	v.Normalize();
	return v;
}
//-----------------------------------------------------------------------
Vector3 Vector3::Max(const Vector3 &a, const Vector3 &b)
{
	return Vector3(
		Mathf::Max<float>(a.x, b.x),
		Mathf::Max<float>(a.y, b.y),
		Mathf::Max<float>(a.z, b.z));
}
//-----------------------------------------------------------------------
Vector3 Vector3::Min(const Vector3 &a, const Vector3 &b)
{
	return Vector3(
		Mathf::Min<float>(a.x, b.x),
		Mathf::Min<float>(a.y, b.y),
		Mathf::Min<float>(a.z, b.z));
}
//-----------------------------------------------------------------------
Vector3 Vector3::Lerp(const Vector3 &from, const Vector3 &to, float t, bool clamp_01)
{
	return Vector3(
		Mathf::Lerp(from.x, to.x, t, clamp_01),
		Mathf::Lerp(from.y, to.y, t, clamp_01),
		Mathf::Lerp(from.z, to.z, t, clamp_01));
}
//-----------------------------------------------------------------------
float Vector3::Angle(const Vector3 &from, const Vector3 &to)
{
	float mod = from.SqrMagnitude() * to.SqrMagnitude();
	float dot = from.Dot(to) / sqrt(mod);
	dot = Mathf::Clamp(dot, -1.0f, 1.0f);

	float deg = acos(dot) * Mathf::Rad2Deg;

	return deg;
}
//-----------------------------------------------------------------------
Vector3 Vector3::operator-() const
{
	float _x = -x;
	float _y = -y;
	float _z = -z;

	return Vector3(_x, _y, _z);
}
//-----------------------------------------------------------------------
Vector3 Vector3::operator+(const Vector3 &v) const
{
	float _x = x + v.x;
	float _y = y + v.y;
	float _z = z + v.z;

	return Vector3(_x, _y, _z);
}
//-----------------------------------------------------------------------
Vector3& Vector3::operator+=(const Vector3 &v)
{
	*this = *this + v;
	return *this;
}
//-----------------------------------------------------------------------
Vector3 Vector3::operator-(const Vector3 &v) const
{
	float _x = x - v.x;
	float _y = y - v.y;
	float _z = z - v.z;

	return Vector3(_x, _y, _z);
}
//-----------------------------------------------------------------------
Vector3 Vector3::operator*(const Vector3 &v) const
{
	float _x = y*v.z - z*v.y;
	float _y = z*v.x - x*v.z;
	float _z = x*v.y - y*v.x;

	return Vector3(_x, _y, _z);
}
//-----------------------------------------------------------------------
Vector3 Vector3::operator*(float v) const
{
	return Vector3(x * v, y * v, z * v);
}
//-----------------------------------------------------------------------
Vector3 Vector3::operator*=(float v)
{
	*this = *this * v;
	return *this;
}
//-----------------------------------------------------------------------
bool Vector3::operator!=(const Vector3 &v) const
{
	return !(*this == v);
}
//-----------------------------------------------------------------------
bool Vector3::operator==(const Vector3 &v) const
{
	return Mathf::FloatEqual(v.x, x) &&
		Mathf::FloatEqual(v.y, y) &&
		Mathf::FloatEqual(v.z, z);
}
//-----------------------------------------------------------------------
float Vector3::Dot(const Vector3 &v) const
{
	return x * v.x + y * v.y + z * v.z;
}
//-----------------------------------------------------------------------
std::string Vector3::ToString() const
{
	return '(' + std::to_string(x) + ',' + std::to_string(y) + ',' + std::to_string(z) + ')';
}
//-----------------------------------------------------------------------
float Vector3::Magnitude() const
{
	return sqrt(SqrMagnitude());
}
//-----------------------------------------------------------------------
float Vector3::SqrMagnitude() const
{
	return x * x + y * y + z * z;
}
//-----------------------------------------------------------------------
float Vector3::Magnitude(const Vector3 &v)
{
	return v.Magnitude();
}
//-----------------------------------------------------------------------
float Vector3::SqrMagnitude(const Vector3 &v)
{
	return v.SqrMagnitude();
}
//-----------------------------------------------------------------------