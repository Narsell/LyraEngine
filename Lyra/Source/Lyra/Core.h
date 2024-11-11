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

#define BIT(x) (1 << x)