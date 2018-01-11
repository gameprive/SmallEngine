#pragma once

class RenderDevice : public Singleton<RenderDevice>
{
public:
	void Init();

	//----------------------------------------------------------------------
	// Render Device Info
	//----------------------------------------------------------------------

	static int32_t GetMaxAnisotropy();
	static int32_t GetMaxTextureSlot();

	static bool SupportsKHRDebug();

	//----------------------------------------------------------------------
	// Render Device Debug Util
	//----------------------------------------------------------------------
	static void LabelObject(GLenum type, GLuint object, const char *name);
	static void PushGroupMarker(const char *name);
	static void PopGroupMarker();

	//----------------------------------------------------------------------
	// Render Resource Create
	//----------------------------------------------------------------------

	// ...

private:

};