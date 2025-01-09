#pragma once

// Compiler intrinsic exports, for now we only support windows 
#ifdef LR_PLATFORM_WINDOWS
	#ifdef LR_BUILD_DLL
		#define LYRA_API //__declspec(dllexport) -- Disabled for now, linking statically
	#else
		#define LYRA_API //__declspec(dllimport) -- Disabled for now, linking statically
	#endif
#else
	#error Lyra only supports windows
#endif

// Assertion macros
// TODO: Investigate using static asserts here
#ifdef LR_ENABLE_ASSERTS
	#define LR_ASSERT(x, ...) { if (!(x)) { LR_FATAL("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define LR_CORE_ASSERT(x, ...) { if (!(x)) { LR_CORE_FATAL("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define LR_ASSERT(x, ...)
	#define LR_CORE_ASSERT(x, ...)
#endif

// Left-shift bit operator macro
#define BIT(x) (1 << x)

// std::bind macro with _1 placehoder for now
#define LR_BIND_EVENT_FN(x) std::bind(x, this, std::placeholders::_1)
