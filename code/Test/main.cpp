// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "Test.h"

//--------------------------------------------------------------------
int main()
{
#if defined(_MSC_VER) && defined(_DEBUG) && FINDMEMORYLEAK
	//int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);// Get current flag
	//flag |= _CRTDBG_LEAK_CHECK_DF;				// Turn on leak-checking bit
	//flag |= _CRTDBG_CHECK_ALWAYS_DF;				// Turn on CrtCheckMemory
	//_CrtSetDbgFlag(flag);							// Set flag to the new value
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	Engine engine;
	engine.Start(new Test);
	return 0;
}
//--------------------------------------------------------------------
