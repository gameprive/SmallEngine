// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "FileSystem.h"
#include "Debug/Log.h"

//--------------------------------------------------------------------
bool FileSystem::ReadFile(const std::string &filename, std::vector<uint8_t> &data)
{
	if ( filename.empty() )
	{
		Log(LevelLog::Error) << "FileSystem: Filename empty!";
		return false;
	}

	std::ifstream file(filename, std::ios::binary);
	if ( !file.is_open() )
	{
		Log(LevelLog::Error) << "FileSystem: Could not open " << filename;
		return false;
	}

	data.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
	
	file.close();

	return true;
}
//--------------------------------------------------------------------
bool FileSystem::WriteFile(const std::string &filename, const std::vector<uint8_t> &data)
{
	if ( filename.empty() )
	{
		Log(LevelLog::Error) << "FileSystem: Filename empty!";
		return false;
	}

	std::ofstream file(filename, std::ios::binary);
	if ( !file.is_open() )
	{
		Log(LevelLog::Error) << "Failed to open file " << filename;
		return false;
	}

	file.write(reinterpret_cast<const char*>(data.data()), static_cast<std::streamsize>(data.size()));

	file.close();
	return true;
}
//--------------------------------------------------------------------