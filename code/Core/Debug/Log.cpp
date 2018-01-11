// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "Log.h"
#include <spdlog/spdlog.h>
//--------------------------------------------------------------------
auto console = spdlog::stdout_color_mt("console");
auto fileLogger = spdlog::basic_logger_mt("basic_logger", "log.txt");
//--------------------------------------------------------------------
// DOC: https://github.com/gabime/spdlog
// DOC: https://github.com/gabime/spdlog/wiki/1.-QuickStart
//--------------------------------------------------------------------
Log::~Log()
{
	if ( m_text.empty() ) return;

	switch ( m_level )
	{
	case LevelLog::Error:
		console->error(m_text);
		fileLogger->error(m_text);
		break;
	case LevelLog::Warning:
		console->warn(m_text);
		fileLogger->warn(m_text);
		break;
	case LevelLog::Info:
		console->info(m_text);
		fileLogger->info(m_text);
		break;

	case LevelLog::Debug:
		console->debug(m_text);
		fileLogger->debug(m_text);
		break;
	default: break;
	}
#if _WIN32
	OutputDebugStringA((m_text + "\n").c_str());
#endif
}
//--------------------------------------------------------------------