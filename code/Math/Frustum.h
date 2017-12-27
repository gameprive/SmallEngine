#pragma once

#include "Matrix4x4.h"
#include "Vector3.h"

enum class ContainsResult
{
	In,
	Out,
	Cross
};

class Frustum
{
public:
	Frustum() {}
	Frustum(const Matrix4x4& mat);
	Frustum(float left, float right, float bottom, float top, float near, float far);

	ContainsResult ContainsPoint(const Vector3 &point) const;
	ContainsResult ContainsSphere(const Vector3 &center, float radius) const;
	ContainsResult ContainsBounds(const Vector3 &min, const Vector3 &max) const;
	ContainsResult ContainsPoints(const std::vector<Vector3>& points, const Matrix4x4* matrix) const;
	float DistanceToPlane(const Vector3 &point, int plane_index) const;

private:
	void NormalizePlanes();

	Vector4 m_planes[6];
};