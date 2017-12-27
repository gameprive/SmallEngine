#pragma once

namespace FileSystem
{
	bool ReadFile(const std::string &filename, std::vector<uint8_t> &data);
	bool WriteFile(const std::string &filename, const std::vector<uint8_t> &data);
}