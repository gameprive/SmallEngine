#pragma once

#include "Math/Vector3.h"

class Geometry
{
public:
	enum class VertexFormat { PTC, PTN, PTNTC, PTNTC_Skinned };

	struct Face
	{
		Face() : a(0), b(0), c(0) {}
		Face(unsigned short fa, unsigned short fb, unsigned short fc) : a(fa), b(fb), c(fc) {}

		unsigned short a, b, c;
	};

	/// Vertex with position, texture coordinate and color.
	struct VertexPTC
	{
		VertexPTC() {}
		VertexPTC(const Vec3& pos, float aU, float aV)
			: position(pos)
			, u(aU)
			, v(aV)
		{
		}

		Vec3 position;
		float u = 0, v = 0;
		Vec4 color = { 1, 1, 1, 1 };
	};

	/// Vertex with position, texcoord, normal, tangent (handedness in .w) and color.
	struct VertexPTNTC
	{
		Vec3 position;
		float u, v;
		Vec3 normal;
		Vec4 tangent;
		Vec4 color;
	};

	struct VertexPTNTC_Skinned
	{
		Vec3 position;
		float u, v;
		Vec3 normal;
		Vec4 tangent;
		Vec4 color;
		Vec4 weights;
		int bones[4];
	};

	/// Vertex with position, texcoord and normal.
	struct VertexPTN
	{
		Vec3 position;
		float u, v;
		Vec3 normal;
	};

	/// Binds the buffer. Must be called before GfxDevice::Draw.
	void Bind() const;

	/// \return Face count.
	int GetFaceCount() const { return elementCount; }

	VertexFormat GetVertexFormat() const { return vertexFormat; }

	/// \return True if the buffer contains geometry ready for rendering.
	bool IsGenerated() const { return elementCount != 0; }

	/// Generates the buffer from supplied geometry.
	/// \param faces Faces.
	/// \param faceCount Face count.
	/// \param vertices Vertices.
	/// \param vertexCount Vertex count.
	void Generate(const Face* faces, int faceCount, const VertexPTC* vertices, int vertexCount);

	/// Generates the buffer from supplied geometry.
	/// \param faces Faces.
	/// \param faceCount Face count.
	/// \param vertices Vertices.
	/// \param vertexCount Vertex count.
	void Generate(const Face* faces, int faceCount, const VertexPTN* vertices, int vertexCount);

	/// Generates the buffer from supplied geometry.
	/// \param faces Faces.
	/// \param faceCount Face count.
	/// \param vertices Vertices.
	/// \param vertexCount Vertex count.
	void Generate(const Face* faces, int faceCount, const VertexPTNTC* vertices, int vertexCount);

	/// Generates the buffer from supplied geometry.
	/// \param faces Faces.
	/// \param faceCount Face count.
	/// \param vertices Vertices.
	/// \param vertexCount Vertex count.
	void Generate(const Face* faces, int faceCount, const VertexPTNTC_Skinned* vertices, int vertexCount);

	/// Sets a graphics API debug name for the buffer, visible in debugging tools. Must be called after Generate().
	/// \param name Name
	void SetDebugName(const char* name);
	
	static const int posChannel = 0;
	static const int uvChannel = 1;
	static const int colorChannel = 2;
	static const int normalChannel = 3;
	static const int tangentChannel = 4;
	static const int boneChannel = 5;
	static const int weightChannel = 6;

private:
	int elementCount = 0;
	VertexFormat vertexFormat = VertexFormat::PTC;

	unsigned vaoId = 0;
	unsigned vboId = 0;
	unsigned iboId = 0;
};