#ifdef CPHI_WINDOWS_PLATFORM
    #ifdef CPHI_BUILD_DLL
		#define COMPHI_API __declspec(dllexport)
    #else		
		#define COMPHI_API __declspec(dllimport)
    #endif
#else
	#error Comphi Support Windows Only
#endif // CPHI_WINDOWS_PLATFORM

#ifdef CPHI_ENABLE_ASSERTS
	#define COMPHI_CORE_ASSERT(b,...) { if(!b){ COMPHI_LOG_CORE_ERROR("Assertion Failed: {0}",__VA_ARGS__); __debugbreak(); } }
	#define COMPHI_ASSERT(b,...) { if(!b){ COMPHI_ERROR("Assertion Failed: {0}",__VA_ARGS__); __debugbreak(); } }
#else
	#define COMPHI_CORE_ASSERT(b,...)
	#define COMPHI_ASSERT(b,...)
#endif

#define BIT(x) (1 << x)

namespace Comphi {
	typedef unsigned int Uint;
}
