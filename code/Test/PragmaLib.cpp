#if _DEBUG
#	pragma comment(lib, "ThirdParty_d.lib")
#	pragma comment(lib, "Core_d.lib")
#	pragma comment(lib, "Renderer_d.lib")
#	pragma comment(lib, "Math_d.lib")
#	pragma comment(lib, "Application_d.lib")
#else
#	pragma comment(lib, "ThirdParty.lib")
#	pragma comment(lib, "Core.lib")
#	pragma comment(lib, "Renderer.lib")
#	pragma comment(lib, "Math.lib")
#	pragma comment(lib, "Application.lib")
#endif
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "OpenAL32.lib")
#pragma comment(lib, "assimp-vc140-mt.lib")