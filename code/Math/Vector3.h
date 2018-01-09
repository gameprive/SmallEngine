#pragma once

struct Vec3
{
	Vec3() {}
	Vec3(Vec3&&) noexcept = default;
	Vec3(const Vec3&) noexcept = default;
	Vec3(float ax, float ay, float az) noexcept : x(ax), y(ay), z(az) {}

	Vec3& operator=(Vec3&&) noexcept = default;

	Vec3 operator/(float f) const
	{
		const float inv = 1.0f / f;
		return Vec3(x * inv, y * inv, z * inv);
	}

	Vec3 operator/(const Vec3& v) const
	{
		return Vec3(x / v.x, y / v.y, z / v.z);
	}

	Vec3 operator-() const
	{
		return Vec3(-x, -y, -z);
	}

	Vec3 operator-(const Vec3& v) const
	{
		return Vec3(x - v.x, y - v.y, z - v.z);
	}

	Vec3 operator+(const Vec3& v) const
	{
		return Vec3(x + v.x, y + v.y, z + v.z);
	}

	Vec3 operator+(float f) const
	{
		return Vec3(x + f, y + f, z + f);
	}

	Vec3& operator-=(const Vec3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;

		return *this;
	}

	Vec3& operator+=(const Vec3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;

		return *this;
	}

	Vec3& operator*=(float f)
	{
		x *= f;
		y *= f;
		z *= f;

		return *this;
	}

	Vec3& operator/=(float f)
	{
		const float inv = 1.0f / f;

		x *= inv;
		y *= inv;
		z *= inv;

		return *this;
	}

	Vec3& operator*=(const Vec3& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;

		return *this;
	}

	Vec3& operator=(const Vec3& v) { x = v.x; y = v.y; z = v.z; return *this; }

	Vec3 operator*(const Vec3& v) const
	{
		return Vec3(x * v.x, y * v.y, z * v.z);
	}

	Vec3 operator*(float f) const
	{
		return Vec3(x * f, y * f, z * f);
	}

	Vec3 Normalized() const
	{
		Vec3 out = *this;
		const float len = Length();

		if ( std::abs(len) < 0.0001f )
		{
			out.x = 1.0f;
			out.y = 0.0f;
			out.z = 0.0f;
			return out;
		}

		out *= (1.0f / len);
		return out;

	}

	// Compares this vector with another vector.
	bool IsAlmost(const Vec3 &v2) const
	{
		const float epsilon = 0.0001f;
		return 
			std::abs(x - v2.x) < epsilon &&
			std::abs(y - v2.y) < epsilon &&
			std::abs(z - v2.z) < epsilon;
	}

	float Length() const { return std::sqrt(x * x + y * y + z * z); }

	void Zero() { x = y = z = 0; }

	static Vec3 Cross(const Vec3& v1, const Vec3& v2)
	{
		return Vec3(v1.y * v2.z - v1.z * v2.y,
			v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x);
	}	

	static float Distance(const Vec3& v1, const Vec3& v2)
	{
		return (v2 - v1).Length();
	}

	static float DistanceSquared(const Vec3& v1, const Vec3& v2)
	{
		const Vec3 sub = v2 - v1;
		return sub.x * sub.x + sub.y * sub.y + sub.z * sub.z;
	}

	static float Dot(const Vec3& v1, const Vec3& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	// Reflects a vector with normal.
	static Vec3 Reflect(const Vec3& vec, const Vec3& normal) { return vec - normal * Dot(normal, vec) * 2; }

	float x = 0;
	float y = 0;
	float z = 0;
};

struct Vec4
{
	Vec4() {}
	Vec4(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
	Vec4(Vec4&&) noexcept = default;
	Vec4(const Vec4&) noexcept = default;
	explicit Vec4(const Vec3& v) : x(v.x), y(v.y), z(v.z), w(1) {}
	Vec4(const Vec3& v, float aW) : x(v.x), y(v.y), z(v.z), w(aW) {}

	// Compares this vector with another vector.
	bool IsAlmost(const Vec4& v2) const
	{
		const float epsilon = 0.0001f;
		return 
			std::abs(x - v2.x) < epsilon &&
			std::abs(y - v2.y) < epsilon &&
			std::abs(z - v2.z) < epsilon &&
			std::abs(w - v2.w) < epsilon;
	}

	Vec4& operator=(const Vec4& v) { x = v.x; y = v.y; z = v.z; w = v.w; return *this; }

	Vec4 operator*(float f) const
	{
		return Vec4(x * f, y * f, z * f, w * f);
	}

	Vec4& operator+=(const Vec4& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;

		return *this;
	}

	Vec4& operator-=(const Vec4& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;

		return *this;
	}

	float Dot(const Vec4& v) const
	{
		return x * v.x + y * v.y + z * v.z + w * v.w;
	}

	float Length() const { return std::sqrt(x * x + y * y + z * z); }

	void Normalize()
	{
		const float len = Length();

		if ( std::abs(len) < 0.0001f )
		{
			x = 1;
			y = 0;
			z = 0;
			return;
		}

		const float iLength = 1 / len;

		x *= iLength;
		y *= iLength;
		z *= iLength;
	}

	float x = 0;
	float y = 0;
	float z = 0;
	float w = 0;
};