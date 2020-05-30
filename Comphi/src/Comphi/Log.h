#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Comphi {
	class CPHI_API Log
	{
	public:
		static void Init();

		static inline std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; };
		static inline std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; };

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}
//CORE
#define LOG_CPHY_CORE_FATAL(...) Comphi::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define LOG_CPHY_CORE_ERROR(...) Comphi::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LOG_CPHY_CORE_WARN(...) Comphi::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LOG_CPHY_CORE_INFO(...) Comphi::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LOG_CPHY_CORE_TRACE(...) Comphi::Log::GetCoreLogger()->trace(__VA_ARGS__)
//CLIENT
#define LOG_CPHY_FATAL(...) Comphi::Log::GetClientLogger()->critical(__VA_ARGS__)
#define LOG_CPHY_ERROR(...) Comphi::Log::GetClientLogger()->error(__VA_ARGS__)
#define LOG_CPHY_WARN(...) Comphi::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_CPHY_INFO(...) Comphi::Log::GetClientLogger()->info(__VA_ARGS__)
#define LOG_CPHY_TRACE(...) Comphi::Log::GetClientLogger()->trace(__VA_ARGS__)



