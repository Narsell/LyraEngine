#pragma once
#include "Core.h"
#include "spdlog/spdlog.h"
#include <memory>

namespace Lyra
{
	class LYRA_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:

		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

	};
}

//CORE LOG MACROS
#define LR_CORE_TRACE(...)   ::Lyra::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LR_CORE_INFO(...)    ::Lyra::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LR_CORE_WARN(...)    ::Lyra::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LR_CORE_ERROR(...)   ::Lyra::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LR_CORE_FATAL(...)   ::Lyra::Log::GetCoreLogger()->critical(__VA_ARGS__)

//CLIENT LOG MACROS
#define LR_TRACE(...)        ::Lyra::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LR_INFO(...)         ::Lyra::Log::GetClientLogger()->info(__VA_ARGS__)
#define LR_WARN(...)         ::Lyra::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LR_ERROR(...)        ::Lyra::Log::GetClientLogger()->error(__VA_ARGS__)
#define LR_FATAL(...)        ::Lyra::Log::GetClientLogger()->critical(__VA_ARGS__)

