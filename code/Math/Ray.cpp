// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "Ray.h"

Ray::Ray(const Vector3 &origin, const Vector3 &direction) :
	m_origin(origin),
	m_direction(Vector3::Normalize(direction))
{
}

Vector3 Ray::GetPoint(float distance) const
{
	return m_origin + m_direction * distance;
}