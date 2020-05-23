#ifdef CPHY_WINDOWS_PLATFORM
    #ifdef CPHY_BUILD_DLL
		#define COMPHY_API __declspec(dllexport)
    #else		
		#define COMPHY_API __declspec(dllimport)
    #endif
#else
	#error Only Comphy Support for Windows
#endif // CPHY_WINDOWS_PLATFORM
