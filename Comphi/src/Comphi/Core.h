#pragma once
#ifdef CPHI_WINDOWS_PLATFORM
	/*#ifdef CPHI_DYNAMIC_LINKING
	    #ifdef CPHI_BUILD_DLL
			#define COMPHI_API __declspec(dllexport)
	    #else		
			#define COMPHI_API __declspec(dllimport)
	    #endif
	#else
		#define	COMPHI_API
	#endif*/
#else
	#error Comphi Support Windows Only
#endif // CPHI_WINDOWS_PLATFORM

#ifdef CPHI_ENABLE_ASSERTS
	#define COMPHILOG_CORE_ASSERT(b,...) { if(!b){ COMPHILOG_CORE_ERROR("Assertion Failed: {0}",__VA_ARGS__); __debugbreak(); } }
	#define COMPHILOG_ASSERT(b,...) { if(!b){ COMPHILOG_ERROR("Assertion Failed: {0}",__VA_ARGS__); __debugbreak(); } }
#else
	#define COMPHILOG_CORE_ASSERT(b,...)
	#define COMPHILOG_ASSERT(b,...)
#endif

#define GLFW_INCLUDE_VULKAN

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#define BIT(x) (1 << x)
#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Comphi {
	typedef uint32_t uint;
	typedef uint64_t uint64;
}
