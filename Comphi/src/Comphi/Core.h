#ifdef CPHI_WINDOWS_PLATFORM
    #ifdef CPHI_BUILD_DLL
		#define CPHI_API __declspec(dllexport)
    #else		
		#define CPHI_API __declspec(dllimport)
    #endif
#else
	#error Comphi Support Windows Only
#endif // CPHI_WINDOWS_PLATFORM

#ifdef CPHI_ENABLE_ASSERTS
	#define CPHI_CORE_ASSERT(b,...) { if(!b){ CPHI_LOG_ERROR("Assertion Failed: {0}",__VA_ARGS__); __debugbreak(); } }
	#define CPHI_ASSERT(b,...) { if(!b){ CPHI_LOG_ERROR("Assertion Failed: {0}",__VA_ARGS__); __debugbreak(); } }
#else
	#define CPHI_CORE_ASSERT(b,...)
	#define CPHI_ASSERT(b,...)
#endif

#define BIT(x) (1 << x)

typedef unsigned int Uint;
