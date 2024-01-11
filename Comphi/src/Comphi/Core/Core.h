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

//Logging
#ifdef CPHI_ENABLE_ASSERTS
	#define COMPHILOG_CORE_ASSERT(b,...) { if(!b){ COMPHILOG_CORE_ERROR("Assertion Failed: {0}",__VA_ARGS__); __debugbreak(); } }
	#define COMPHILOG_ASSERT(b,...) { if(!b){ COMPHILOG_ERROR("Assertion Failed: {0}",__VA_ARGS__); __debugbreak(); } }
#else
	#define COMPHILOG_CORE_ASSERT(b,...)
	#define COMPHILOG_ASSERT(b,...)
#endif

//Debug Vulkan Logger
#ifndef NDEBUG
	#define NDEBUG_Logger
#endif

//Let GLFW include Vulkan	
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32

//GLM Defines
#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

//Events
#define BIT(x) (1 << x)
#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

//vk Macros
#define vkCheckError(x) if(x != VK_SUCCESS)

namespace Comphi {
	typedef uint32_t uint; //TODO: change to uint32
	typedef uint64_t uint64;
}
