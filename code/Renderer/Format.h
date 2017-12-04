#pragma once

enum class DataType : uint8_t
{
	Int8,
	UInt8,
	Int16,
	UInt16,	
	Int32,
	UInt32,	
	Float,	
	Double,
};

enum class VectorType : uint8_t
{
	Float = 0,	// 1-Dimensional single precision floating-point vector (float in GLSL, float in HLSL).
	Float2,		// 2-Dimensional single precision floating-point vector (vec2 in GLSL, float2 in HLSL).
	Float3,		// 3-Dimensional single precision floating-point vector (vec3 in GLSL, float3 in HLSL).
	Float4,		// 4-Dimensional single precision floating-point vector (vec4 in GLSL, float4 in HLSL).
	Double,		// 1-Dimensional double precision floating-point vector (double in GLSL, double in HLSL).
	Double2,	// 2-Dimensional double precision floating-point vector (dvec2 in GLSL, double2 in HLSL).
	Double3,	// 3-Dimensional double precision floating-point vector (dvec3 in GLSL, double3 in HLSL).
	Double4,	// 4-Dimensional double precision floating-point vector (dvec4 in GLSL, double4 in HLSL).
	Int,		// 1-Dimensional signed integer vector (int in GLSL, int in HLSL).
	Int2,		// 2-Dimensional signed integer vector (ivec2 in GLSL, int2 in HLSL).
	Int3,		// 3-Dimensional signed integer vector (ivec3 in GLSL, int3 in HLSL).
	Int4,		// 4-Dimensional signed integer vector (ivec4 in GLSL, int4 in HLSL).
	UInt,		// 1-Dimensional unsigned integer vector (uint in GLSL, uint in HLSL).
	UInt2,		// 2-Dimensional unsigned integer vector (uvec2 in GLSL, uint2 in HLSL).
	UInt3,		// 3-Dimensional unsigned integer vector (uvec3 in GLSL, uint3 in HLSL).
	UInt4,		// 4-Dimensional unsigned integer vector (uvec4 in GLSL, uint4 in HLSL).
};

uint32_t DataTypeSize(const DataType dataType);
uint32_t VectorTypeSize(const VectorType vectorType);
void VectorTypeFormat(const VectorType vectorType, DataType &dataType, uint8_t &components);