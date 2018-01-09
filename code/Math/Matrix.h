#pragma once

struct Vec3;
struct Vec4;

struct alignas(16) Matrix44
{
	static const Matrix44 identity;

	/* Converts depth coordinates from [-1,1] to [0,1]. This is needed to sample the shadow map texture. */
	static const Matrix44 bias;
	
	static void Invert(const Matrix44& matrix, Matrix44& out);
	static void InverseTranspose(const float m[16], float* out);
	static void Multiply(const Matrix44& a, const Matrix44& b, Matrix44& out);

	// Multiplies a matrix with a vector.
	static void TransformPoint(const Vec4& vec, const Matrix44& mat, Vec4* out);

	// Multiplies a matrix with a vector. Vector's missing w is treated as 1.
	static void TransformPoint(const Vec3& vec, const Matrix44& mat, Vec3* out);

	// Multiplies a matrix with a vector. Vector's missing w is treated as 0.
	static void TransformDirection(const Vec3& dir, const Matrix44& mat, Vec3* out);

	Matrix44()
	{
		MakeIdentity();
	}

	Matrix44(float xDeg, float yDeg, float zDeg)
	{
		MakeRotationXYZ(xDeg, yDeg, zDeg);
	}

	Matrix44(const Matrix44& other);
	explicit Matrix44(const float* data);

	Matrix44& operator=(const Matrix44& other)
	{
		if ( this == &other )
		{
			return *this;
		}

		InitFrom(&other.m[0]);
		return *this;
	}

	void Scale(float x, float y, float z);

	void InitFrom(const float* data);

	void MakeIdentity();

	void SetTranslation(const Vec3& translation);

	// Makes a lookat matrix (left-handed).
	void MakeLookAt(const Vec3& eye, const Vec3& center, const Vec3& up);

	// Sets perspective projection (left-handed).
	// Calculated as in gluPerspective (http://www.opengl.org/wiki/GluPerspective_code)
	void MakeProjection(float fovDegrees, float aspect, float nearDepth, float farDepth);

	void MakeProjection(float left, float right, float bottom, float top, float nearDepth, float farDepth);

	void MakeRotationXYZ(float xDeg, float yDeg, float zDeg);

	void Transpose(Matrix44& out) const;

	void Translate(const Vec3& v);

	float m[16];
};