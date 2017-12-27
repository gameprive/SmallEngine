#pragma once

#include "Vector3.h"

class Ray
{
public:
	Ray(const Vector3 &origin, const Vector3 &direction);
	const Vector3 &GetOrigin() const { return m_origin; }
	void SetOrigin(const Vector3 &origin) { m_origin = origin; }
	const Vector3 &GetDirection() const { return m_direction; }
	void SetDirection(const Vector3 &direction) { m_direction = Vector3::Normalize(direction); }
	Vector3 GetPoint(float distance) const;

private:
	Vector3 m_origin;
	Vector3 m_direction;
};