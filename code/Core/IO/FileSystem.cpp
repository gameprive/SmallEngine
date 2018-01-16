// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "FileSystem.h"
#include "Debug/Log.h"

//--------------------------------------------------------------------
FileReader::FileReader(const std::string &file)
{
	isLoaded = FileSystem::ReadFile(file, data);
	fileName = file;
	m_top = data.size();
}
//--------------------------------------------------------------------
int64_t FileReader::Seek(int64_t offset, Whence whence)
{
	switch ( whence )
	{
	case Whence::Begin:
		m_pos = Clamp(offset, 0, m_top);
		break;

	case Whence::Current:
		m_pos = Clamp(m_pos + offset, 0, m_top);
		break;

	case Whence::End:
		m_pos = Clamp(m_top - offset, 0, m_top);
		break;
	}

	return m_pos;
}
//--------------------------------------------------------------------
uint32_t FileReader::Read(void *_data, uint32_t _size)
{
	int32_t remainder = m_top - m_pos;
	uint32_t size = std::min(_size, uint32_t(std::min(remainder, INT32_MAX)));

	if ( size != _size )
	{
		LogError() << "FileReader: read truncated.";
		return 0;
	}		

	memcpy(_data, &data[m_pos], size);
	m_pos += size;
	return size;
}
//--------------------------------------------------------------------
bool FileSystem::ReadFile(const std::string &filename, std::vector<uint8_t> &data)
{
	if ( filename.empty() )
	{
		LogError() << "FileSystem: Filename empty!";
		return false;
	}

	std::ifstream file(filename, std::ios::binary);
	if ( !file.is_open() )
	{
		LogError() << "FileSystem: Could not open " << filename;
		file.close();
		return false;
	}
	data.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

	file.close();
	
	if ( data.empty() )
	{
		LogError() << "FileSystem: data empty!";
		return false;
	}	

	return true;
}
//--------------------------------------------------------------------
bool FileSystem::WriteFile(const std::string &filename, const std::vector<uint8_t> &data)
{
	if ( filename.empty() )
	{
		LogError() << "FileSystem: Filename empty!";
		return false;
	}

	std::ofstream file(filename, std::ios::binary);
	if ( !file.is_open() )
	{
		LogError() << "Failed to open file " << filename;
		return false;
	}

	file.write(reinterpret_cast<const char*>(data.data()), static_cast<std::streamsize>(data.size()));

	file.close();
	return true;
}
//--------------------------------------------------------------------