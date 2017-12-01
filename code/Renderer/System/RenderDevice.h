#pragma once

class RenderDevice : public Singleton<RenderDevice>
{
public:
	void Init();

	//-------------------------------------------------------------------------
	// Render Device Info
	//-------------------------------------------------------------------------

	int32_t GetMaxAnisotropy() const;
	int32_t GetMaxTextureSlot() const;
	int32_t GetMaxVertexConstantBuffer() const;
	int32_t GetMaxPixelConstantBuffer() const;

	//-------------------------------------------------------------------------
	// Render Resource Create
	//-------------------------------------------------------------------------

	// ...

private:
	int32_t m_deviceMaxAnisotropy = 0;
	int32_t m_maxTextureSlot = 0;
	int32_t m_maxVertexConstantBuffers = 0;
	int32_t m_maxPixelConstantBuffers = 0;
};