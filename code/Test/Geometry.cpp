// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "Geometry.h"
#include "Statistics.h"
#include "HasExtension.h"

TODO(" glDeleteVertexArrays, glDeleteBuffers");

namespace Global
{
	GLuint activeVao = 0;
}

void Geometry::Generate(const Face* faces, int faceCount, const VertexPTC* vertices, int vertexCount)
{
	vertexFormat = VertexFormat::PTC;
	elementCount = faceCount * 3;

	if ( vaoId == 0 )
	{
		glGenVertexArrays(1, &vaoId);
	}

	glBindVertexArray(vaoId);

	if ( vboId == 0 )
	{
		glGenBuffers(1, &vboId);
	}

	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(VertexPTC), vertices, GL_STATIC_DRAW);

	if ( iboId == 0 )
	{
		glGenBuffers(1, &iboId);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, faceCount * sizeof(Face), faces, GL_STATIC_DRAW);

	// Position.
	glEnableVertexAttribArray(posChannel);
	glVertexAttribPointer(posChannel, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPTC), nullptr);

	// TexCoord.
	glEnableVertexAttribArray(uvChannel);
	glVertexAttribPointer(uvChannel, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPTC), (GLvoid*)offsetof(struct VertexPTC, u));

	// Color.
	glEnableVertexAttribArray(colorChannel);
	glVertexAttribPointer(colorChannel, 4, GL_FLOAT, GL_FALSE, sizeof(VertexPTC), (GLvoid*)offsetof(struct VertexPTC, color));
}

void Geometry::SetDebugName(const char* name)
{
	if ( HasExtension("GL_KHR_debug") )
	{
		glObjectLabel(GL_BUFFER, vboId, -1, std::strlen(name) < 256 ? name : "long vb name");
	}
}

void Geometry::Generate(const Face* faces, int faceCount, const VertexPTN* vertices, int vertexCount)
{
	vertexFormat = VertexFormat::PTN;
	elementCount = faceCount * 3;

	if ( vaoId == 0 )
	{
		glGenVertexArrays(1, &vaoId);
	}

	glBindVertexArray(vaoId);

	if ( vboId == 0 )
	{
		glGenBuffers(1, &vboId);
	}

	glBindBuffer(GL_ARRAY_BUFFER, vboId);

	if ( HasExtension("GL_KHR_debug") )
	{
		glObjectLabel(GL_BUFFER, vboId, -1, "vbo");
	}

	if ( HasExtension("GL_ARB_buffer_storage") )
	{
		const GLbitfield flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
		glBufferStorage(GL_ARRAY_BUFFER, vertexCount * sizeof(VertexPTN), vertices, flags);
	}
	else
	{
		glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(VertexPTN), vertices, GL_STATIC_DRAW);
	}

	if ( iboId == 0 )
	{
		glGenBuffers(1, &iboId);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);

	if ( HasExtension("GL_KHR_debug") )
	{
		glObjectLabel(GL_BUFFER, iboId, -1, "ibo");
	}

	if ( HasExtension("GL_ARB_buffer_storage") )
	{
		const GLbitfield flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
		glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, faceCount * sizeof(Face), faces, flags);
	}
	else
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, faceCount * sizeof(Face), faces, GL_STATIC_DRAW);
	}

	// Position.
	glEnableVertexAttribArray(posChannel);
	glVertexAttribPointer(posChannel, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPTN), nullptr);

	// TexCoord.
	glEnableVertexAttribArray(uvChannel);
	glVertexAttribPointer(uvChannel, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPTN), (GLvoid*)offsetof(struct VertexPTN, u));

	// Normal.
	glEnableVertexAttribArray(normalChannel);
	glVertexAttribPointer(normalChannel, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPTN), (GLvoid*)offsetof(struct VertexPTN, normal));
}

void Geometry::Generate(const Face* faces, int faceCount, const VertexPTNTC* vertices, int vertexCount)
{
	vertexFormat = VertexFormat::PTNTC;
	elementCount = faceCount * 3;

	if ( vaoId == 0 )
	{
		glGenVertexArrays(1, &vaoId);
	}

	glBindVertexArray(vaoId);

	if ( vboId == 0 )
	{
		glGenBuffers(1, &vboId);
	}

	glBindBuffer(GL_ARRAY_BUFFER, vboId);

	if ( HasExtension("GL_ARB_buffer_storage") )
	{
		const GLbitfield flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
		glBufferStorage(GL_ARRAY_BUFFER, vertexCount * sizeof(VertexPTNTC), vertices, flags);
	}
	else
	{
		glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(VertexPTNTC), vertices, GL_STATIC_DRAW);
	}

	if ( iboId == 0 )
	{
		glGenBuffers(1, &iboId);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);

	if ( HasExtension("GL_ARB_buffer_storage") )
	{
		const GLbitfield flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
		glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, faceCount * sizeof(Face), faces, flags);
	}
	else
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, faceCount * sizeof(Face), faces, GL_STATIC_DRAW);
	}

	// Position.
	glEnableVertexAttribArray(posChannel);
	glVertexAttribPointer(posChannel, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPTNTC), nullptr);

	// TexCoord.
	glEnableVertexAttribArray(uvChannel);
	glVertexAttribPointer(uvChannel, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPTNTC), (GLvoid*)offsetof(struct VertexPTNTC, u));

	// Normal.
	glEnableVertexAttribArray(normalChannel);
	glVertexAttribPointer(normalChannel, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPTNTC), (GLvoid*)offsetof(struct VertexPTNTC, normal));

	// Tangent.
	glEnableVertexAttribArray(tangentChannel);
	glVertexAttribPointer(tangentChannel, 4, GL_FLOAT, GL_FALSE, sizeof(VertexPTNTC), (GLvoid*)offsetof(struct VertexPTNTC, tangent));

	// Color.
	glEnableVertexAttribArray(colorChannel);
	glVertexAttribPointer(colorChannel, 4, GL_FLOAT, GL_FALSE, sizeof(VertexPTNTC), (GLvoid*)offsetof(struct VertexPTNTC, color));
}

void Geometry::Generate(const Face* faces, int faceCount, const VertexPTNTC_Skinned* vertices, int vertexCount)
{
	vertexFormat = VertexFormat::PTNTC_Skinned;
	elementCount = faceCount * 3;

	if ( vaoId == 0 )
	{
		glGenVertexArrays(1, &vaoId);
	}

	glBindVertexArray(vaoId);

	if ( vboId == 0 )
	{
		glGenBuffers(1, &vboId);
	}

	glBindBuffer(GL_ARRAY_BUFFER, vboId);

	if ( HasExtension("GL_ARB_buffer_storage") )
	{
		const GLbitfield flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
		glBufferStorage(GL_ARRAY_BUFFER, vertexCount * sizeof(VertexPTNTC_Skinned), vertices, flags);
	}
	else
	{
		glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(VertexPTNTC_Skinned), vertices, GL_STATIC_DRAW);
	}

	if ( iboId == 0 )
	{
		glGenBuffers(1, &iboId);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);

	if ( HasExtension("GL_ARB_buffer_storage") )
	{
		const GLbitfield flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
		glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, faceCount * sizeof(Face), faces, flags);
	}
	else
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, faceCount * sizeof(Face), faces, GL_STATIC_DRAW);
	}

	// Position.
	glEnableVertexAttribArray(posChannel);
	glVertexAttribPointer(posChannel, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPTNTC_Skinned), nullptr);

	// TexCoord.
	glEnableVertexAttribArray(uvChannel);
	glVertexAttribPointer(uvChannel, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPTNTC_Skinned), (GLvoid*)offsetof(struct VertexPTNTC_Skinned, u));

	// Normal.
	glEnableVertexAttribArray(normalChannel);
	glVertexAttribPointer(normalChannel, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPTNTC_Skinned), (GLvoid*)offsetof(struct VertexPTNTC_Skinned, normal));

	// Tangent.
	glEnableVertexAttribArray(tangentChannel);
	glVertexAttribPointer(tangentChannel, 4, GL_FLOAT, GL_FALSE, sizeof(VertexPTNTC_Skinned), (GLvoid*)offsetof(struct VertexPTNTC_Skinned, tangent));

	// Color.
	glEnableVertexAttribArray(colorChannel);
	glVertexAttribPointer(colorChannel, 4, GL_FLOAT, GL_FALSE, sizeof(VertexPTNTC_Skinned), (GLvoid*)offsetof(struct VertexPTNTC_Skinned, color));

	// Bones.
	glEnableVertexAttribArray(boneChannel);
	glVertexAttribIPointer(boneChannel, 4, GL_INT, sizeof(VertexPTNTC_Skinned), (GLvoid*)offsetof(struct VertexPTNTC_Skinned, bones));

	// Weights.
	glEnableVertexAttribArray(weightChannel);
	glVertexAttribPointer(weightChannel, 4, GL_FLOAT, GL_FALSE, sizeof(VertexPTNTC_Skinned), (GLvoid*)offsetof(struct VertexPTNTC_Skinned, weights));
}

void Geometry::Bind() const
{
	if ( Global::activeVao != vaoId )
	{
		glBindVertexArray(vaoId);
		Global::activeVao = vaoId;
		Statistics::IncVertexBufferBinds();
	}
}