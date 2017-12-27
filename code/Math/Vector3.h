#pragma once

class Vector2;

class Vector3
{
public:
	static const Vector3 Zero;
	static const Vector3 One;

	explicit Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	Vector3(const Vector2 &v2);
	Vector3 operator-() const;
	Vector3 operator+(const Vector3 &v) const;
	Vector3& operator+=(const Vector3 &v);
	Vector3 operator-(const Vector3 &v) const;
	Vector3 operator*(const Vector3 &v) const;
	Vector3 operator*(float v) const;
	Vector3 operator*=(float v);
	bool operator!=(const Vector3 &v) const;
	bool operator==(const Vector3 &v) const;
	float Dot(const Vector3 &v) const;
	std::string ToString() const;
	void Normalize();
	float Magnitude() const;
	float SqrMagnitude() const;
	
	static Vector3 Normalize(const Vector3 &value);
	static float Magnitude(const Vector3 &v);
	static float SqrMagnitude(const Vector3 &v);
	static Vector3 Max(const Vector3 &a, const Vector3 &b);
	static Vector3 Min(const Vector3 &a, const Vector3 &b);
	static Vector3 Lerp(const Vector3 &from, const Vector3 &to, float t, bool clamp_01 = true);
	static float Angle(const Vector3 &from, const Vector3 &to);

	float x;
	float y;
	float z;
};