// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "OldImage.h"
#include "Core/IO/FileSystem.h"
#define STBI_NO_PSD
#define STBI_NO_HDR
#define STBI_NO_PIC
#define STBI_NO_GIF
#define STBI_NO_PNM
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

//--------------------------------------------------------------------
bool OldImage::Init(const std::string &filename)
{
	m_filename = filename;

	FileReader data(m_filename);
	if ( !data.isLoaded )
		return false;
	
	return Init(data.data);
}
//--------------------------------------------------------------------
bool OldImage::Init(const std::vector<uint8_t> &data)
{
	TODO("определить формат пикселя");
	TODO("объединить с DDSLoader");

	int width, height, comp;
	stbi_uc *tempData = stbi_load_from_memory(data.data(), static_cast<int>(data.size()), &width, &height, &comp, STBI_rgb_alpha);
	if ( !tempData )
	{
		Log(LevelLog::Error) << "Failed to open texture file " << m_filename << ", reason: " << stbi_failure_reason();
		return false;
	}

	m_data.assign(tempData, tempData + (width * height * STBI_rgb_alpha));

	stbi_image_free(tempData);

	m_width = width;
	m_height = height;

	return true;
}
//--------------------------------------------------------------------
