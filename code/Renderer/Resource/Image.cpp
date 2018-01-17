// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "Image.h"
#include "Core/IO/FileSystem.h"
#include "DDSImage.h"
#define STBI_NO_STDIO
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

//--------------------------------------------------------------------
bool Image::Init(const std::string &filename)
{
	m_filename = filename;

	FileReader data(m_filename);
	if ( !data.isLoaded ) return false;

	uint32_t magic;
	data.Read(magic);
	data.Seek(0);

	if ( magic == DDS_MAGIC )
	{
	}
	// Load STB
	else
	{
		if ( !loadSTB(data) ) return false;
	}	

	return true;
}
//--------------------------------------------------------------------
bool Image::loadSTB(const FileReader &fileData)
{
	const int size = static_cast<int>(fileData.data.size());
	const int isHDR = stbi_is_hdr_from_memory(fileData.data.data(), size);

	void *tempData;
	int width = 0;
	int height = 0;
	int comp = 0;

	if ( isHDR )
		tempData = stbi_loadf_from_memory(fileData.data.data(), size, &width, &height, &comp, 4);
	else
		tempData = stbi_load_from_memory(fileData.data.data(), size, &width, &height, &comp, 0);

	if ( !tempData )
	{
		LogError() << "Failed to open texture file " << m_filename << ", reason: " << stbi_failure_reason();
		return false;
	}

	m_format = TextureFormat::RGBA8;
	if ( isHDR )
	{
		m_format = TextureFormat::RGBA32F;
	}
	else
	{
		switch ( comp )
		{
		case 1: m_format = TextureFormat::R8; break;
		case 2: m_format = TextureFormat::RG8; break;
		case 3: m_format = TextureFormat::RGB8; break;
		default: break;
		}
	}

	m_data.assign((const uint8_t*)tempData, (const uint8_t*)tempData + (width * height * comp));

	//int width, height, comp;
	//stbi_uc *tempData = stbi_load_from_memory(fileData.data.data(), static_cast<int>(fileData.data.size()), &width, &height, &comp, STBI_rgb_alpha);
	//if ( !tempData )
	//{
	//	Log(LevelLog::Error) << "Failed to open texture file " << m_filename << ", reason: " << stbi_failure_reason();
	//	return false;
	//}

	//m_data.assign(tempData, tempData + (width * height * STBI_rgb_alpha));

	stbi_image_free(tempData);

	m_width = width;
	m_height = height;

	return true;
}
//--------------------------------------------------------------------