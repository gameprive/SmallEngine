// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "Mathf.h"

const float Mathf::Epsilon = 0.00001f;
const float Mathf::PI = 3.1415926f;
const float Mathf::Deg2Rad = 0.0174533f;
const float Mathf::Rad2Deg = 57.2958f;
const float Mathf::MaxFloatValue = 3.402823466e+38F;
const float Mathf::MinFloatValue = -Mathf::MaxFloatValue;

float Mathf::Lerp(float from, float to, float t, bool clamp_01)
{
	if ( clamp_01 )
	{
		t = Mathf::Clamp01(t);
	}

	return from + (to - from) * t;
}

float Mathf::Round(float f)
{
	float up = ceil(f);
	float down = floor(f);

	if ( f - down < 0.5f )
	{
		return down;
	}
	else if ( up - f < 0.5f )
	{
		return up;
	}
	else
	{
		return ((((int)up) % 2) == 0 ? up : down);
	}
}

int Mathf::RoundToInt(float f)
{
	return (int)Round(f);
}

float Mathf::RandomRange(float min, float max)
{
	long long rand_max = (long long)RAND_MAX + 1;
	double rand_01 = rand() / (double)rand_max;

	return (float)(min + rand_01 * (max - min));
}

int Mathf::RandomRange(int min, int max)
{
	return (int)(min + RandomRange(0.0f, 1.0f) * (max - min));
}

bool Mathf::RayPlaneIntersection(const Ray& ray, const Vector3 &plane_normal, const Vector3 &plane_point, float& ray_length)
{
	float n_dot_d = ray.GetDirection().Dot(plane_normal);

	if ( FloatEqual(n_dot_d, 0) )
	{
		return false;
	}

	ray_length = (plane_point - ray.GetOrigin()).Dot(plane_normal) / n_dot_d;

	return true;
}

bool Mathf::RayBoundsIntersection(const Ray& ray, const Bounds& box, float& ray_length)
{
	const Vector3 &dir = ray.GetDirection();

	float t_x_min;
	float t_x_max;
	float t_y_min;
	float t_y_max;
	float t_z_min;
	float t_z_max;

	if ( FloatEqual(dir.x, 0) )
	{
		t_x_min = MinFloatValue;
		t_x_max = MaxFloatValue;
	}
	else
	{
		float inv_x = 1 / dir.x;
		const Vector3 &box_min = box.Min();
		const Vector3 &box_max = box.Max();

		if ( inv_x > 0 )
		{
			t_x_min = (box_min.x - ray.GetOrigin().x) * inv_x;
			t_x_max = (box_max.x - ray.GetOrigin().x) * inv_x;
		}
		else
		{
			t_x_min = (box_max.x - ray.GetOrigin().x) * inv_x;
			t_x_max = (box_min.x - ray.GetOrigin().x) * inv_x;
		}
	}

	if ( FloatEqual(dir.y, 0) )
	{
		t_y_min = MinFloatValue;
		t_y_max = MaxFloatValue;
	}
	else
	{
		float inv_y = 1 / dir.y;
		const Vector3 &box_min = box.Min();
		const Vector3 &box_max = box.Max();

		if ( inv_y > 0 )
		{
			t_y_min = (box_min.y - ray.GetOrigin().y) * inv_y;
			t_y_max = (box_max.y - ray.GetOrigin().y) * inv_y;
		}
		else
		{
			t_y_min = (box_max.y - ray.GetOrigin().y) * inv_y;
			t_y_max = (box_min.y - ray.GetOrigin().y) * inv_y;
		}
	}

	if ( FloatEqual(dir.z, 0) )
	{
		t_z_min = MinFloatValue;
		t_z_max = MaxFloatValue;
	}
	else
	{
		float inv_z = 1 / dir.z;
		const Vector3 &box_min = box.Min();
		const Vector3 &box_max = box.Max();

		if ( inv_z > 0 )
		{
			t_z_min = (box_min.z - ray.GetOrigin().z) * inv_z;
			t_z_max = (box_max.z - ray.GetOrigin().z) * inv_z;
		}
		else
		{
			t_z_min = (box_max.z - ray.GetOrigin().z) * inv_z;
			t_z_max = (box_min.z - ray.GetOrigin().z) * inv_z;
		}
	}

	std::vector<float> mins;
	mins.push_back(t_x_min);
	mins.push_back(t_y_min);
	mins.push_back(t_z_min);

	float min = Max<float>(mins);

	std::vector<float> maxs;
	maxs.push_back(t_x_max);
	maxs.push_back(t_y_max);
	maxs.push_back(t_z_max);

	float max = Min<float>(maxs);

	if ( min > max )
	{
		return false;
	}

	if ( !box.Contains(ray.GetPoint(min)) )
	{
		return false;
	}

	ray_length = min;

	return true;
}