#pragma once

enum struct Whence
{
	Begin,
	Current,
	End,
};

// TODO: FileReader может наследоваться от MemoryReader
struct FileReader
{
	FileReader(const std::string &name);

	int64_t Seek(int64_t offset, Whence whence = Whence::Begin);
	
	uint32_t Read(void *data, uint32_t size);
	
	template <typename T> 
	uint32_t Read(T &value)
	{
		return Read(&value, sizeof(T));
	}
	
	std::vector<uint8_t> data;
	std::string fileName;
	bool isLoaded = false;

private:
	int64_t m_top;
	int64_t m_pos = 0;
};

namespace FileSystem
{
	bool ReadFile(const std::string &filename, std::vector<uint8_t> &data);
	bool WriteFile(const std::string &filename, const std::vector<uint8_t> &data);
}