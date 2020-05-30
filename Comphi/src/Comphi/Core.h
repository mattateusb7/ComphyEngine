#ifdef CPHI_WINDOWS_PLATFORM
    #ifdef CPHI_BUILD_DLL
		#define CPHI_API __declspec(dllexport)
    #else		
		#define CPHI_API __declspec(dllimport)
    #endif
#else
	#error Only Comphi Support for Windows
#endif // CPHI_WINDOWS_PLATFORM

#define BIT(x) (1 << x)

typedef unsigned int Uint;
