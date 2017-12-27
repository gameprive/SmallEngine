﻿#pragma once

#include "Vector3.h"
#include "Ray.h"
#include "Bounds.h"

class Mathf
{
public:
	static const float Epsilon;
	static const float PI;
	static const float Deg2Rad;
	static const float Rad2Deg;
	static const float MaxFloatValue;
	static const float MinFloatValue;

	template<class T>
	static T Max(T a, T b) { return a > b ? a : b; }
	template<class T>
	inline static T Max(const std::vector<T>& a);
	template<class T>
	static T Min(T a, T b) { return a < b ? a : b; }
	template<class T>
	inline static T Min(const std::vector<T>& a);
	static float Clamp01(float value) { return Min<float>(Max<float>(value, 0), 1); }
	template<class T>
	static T Clamp(T value, T min, T max) { return Min(Max(value, min), max); }
	static float Lerp(float from, float to, float t, bool clamp_01 = true);
	static bool FloatEqual(float a, float b) { return fabs(a - b) < Epsilon; }
	static float Round(float f);//ЛДЙбОеИл
	static int RoundToInt(float f);
	static float Sign(float f) { return f < 0 ? -1.0f : 1.0f; }
	template<class T>
	static void Swap(T& a, T& b) { T temp = a; a = b; b = temp; }
	static float RandomRange(float min, float max);
	static int RandomRange(int min, int max);
	static float Log2(float x) { return logf(x) / logf(2); }
	static int Abs(int v) { return (int)fabsf((float)v); }
	static bool RayPlaneIntersection(const Ray& ray, const Vector3 &plane_normal, const Vector3 &plane_point, float& ray_length);
	static bool RayBoundsIntersection(const Ray& ray, const Bounds& box, float& ray_length);
};

template<class T>
T Mathf::Max(const std::vector<T>& a)
{
	T max = a[0];

	for ( size_t i = 1; i < a.size(); i++ )
	{
		if ( a[i] > max )
		{
			max = a[i];
		}
	}

	return max;
}

template<class T>
T Mathf::Min(const std::vector<T>& a)
{
	T min = a[0];

	for ( size_t i = 1; i < a.size(); i++ )
	{
		if ( a[i] < min )
		{
			min = a[i];
		}
	}

	return min;
}