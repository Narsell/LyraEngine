#pragma once

#ifdef LR_PLATFORM_WINDOWS
	#ifdef LR_BUILD_DLL
		#define LYRA_API __declspec(dllexport)
	#else
		#define LYRA_API __declspec(dllimport)
	#endif
#else
	#error Lyra only supports windows
#endif

#ifdef LR_ENABLE_ASSERTS
	#define LR_ASSERT(x, ...) { if (!(x)) { LR_FATAL("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define LR_CORE_ASSERT(x, ...) { if (!(x)) { LR_CORE_FATAL("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define LR_ASSERT(x, ...)
	#define LR_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)
