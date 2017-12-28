﻿// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "Format.h"

//--------------------------------------------------------------------
uint32_t DataTypeSize(const DataType dataType)
{
	switch ( dataType )
	{
	case DataType::Int8:
	case DataType::UInt8:
		return 1;
	case DataType::Int16:
	case DataType::UInt16:
		return 2;
	case DataType::Int32:
	case DataType::UInt32:
	case DataType::Float:
		return 4;
	case DataType::Double:
		return 8;
	}
	return 0;
}
//--------------------------------------------------------------------
uint32_t VectorTypeSize(const VectorType vectorType)
{
	DataType dataType = DataType::Float;
	uint8_t components = 0;
	VectorTypeFormat(vectorType, dataType, components);
	return (DataTypeSize(dataType) * components);
}
//--------------------------------------------------------------------
void VectorTypeFormat(const VectorType vectorType, DataType &dataType, uint8_t &components)
{
	uint8_t vectorTypeIdx = static_cast<uint8_t>(vectorType);
	const uint8_t componentsIdx = vectorTypeIdx % 4;
	vectorTypeIdx /= 4;
	if ( vectorTypeIdx < 4 )
	{	
		static const DataType vecDataTypes[] = { DataType::Float, DataType::Double, DataType::Int32, DataType::UInt32 };
		dataType = vecDataTypes[vectorTypeIdx];
		components = (componentsIdx + 1);
	}
}
//--------------------------------------------------------------------
