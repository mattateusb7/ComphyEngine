#include "cphipch.h"
#include "Log.h"

namespace Comphi {

	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init() {

		spdlog::set_pattern("%^[%H:%M:%S] %n >> %v%$");
		
		s_CoreLogger = spdlog::stdout_color_mt("_cphi_");
		s_CoreLogger->set_level(spdlog::level::trace);
		
		s_ClientLogger = spdlog::stdout_color_mt("CLIENT");
		s_ClientLogger->set_level(spdlog::level::trace);
	}

}