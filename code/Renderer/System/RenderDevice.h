#pragma once

class RenderDevice : public Singleton<RenderDevice>
{
public:
	void Init();

	//----------------------------------------------------------------------
	// Device Info
	//----------------------------------------------------------------------

	static int32_t GetMaxAnisotropy();
	static int32_t GetMaxTextureSlot();

	static bool SupportsKHRDebug();
	static bool SupportsTextureFilterAnisotropic();
	static bool SupportsTextureCompressionS3TC();
	static bool SupportsBufferStorage();
	
	//----------------------------------------------------------------------
	// Device Debug Util
	//----------------------------------------------------------------------
	static void LabelObject(GLenum type, GLuint object, const char *name);
	static void PushGroupMarker(const char *name);
	static void PopGroupMarker();

	//----------------------------------------------------------------------
	// Resource Create
	//----------------------------------------------------------------------

	// ...

private:
	void prepareOpenGL();
};