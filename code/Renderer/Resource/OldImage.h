#pragma once

class OldImage : public Noncopyable
{
public:
	bool Init(const std::string &filename);
	bool Init(const std::vector<uint8_t> &data);

	const std::vector<uint8_t>& GetData() const { return m_data; }

	uint32_t GetWidth() const { return m_width; }
	uint32_t GetHeight() const { return m_height; }

private:
	std::string m_filename;
	uint32_t m_width = 0;
	uint32_t m_height = 0;
	std::vector<uint8_t> m_data;
};