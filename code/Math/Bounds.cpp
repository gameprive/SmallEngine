// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "Bounds.h"

Bounds::Bounds(const Vector3 &min, const Vector3 &max) :
	m_min(min),
	m_max(max)
{
}

bool Bounds::Contains(const Vector3 &point) const
{
	return !(point.x < m_min.x || point.y < m_min.y || point.z < m_min.z ||
		point.x > m_max.x || point.y > m_max.y || point.z > m_max.z);
}