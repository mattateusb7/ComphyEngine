#pragma once
// For use by Comphi Applications

//Utilities
#include "cphipch.h"
#include "Comphi/Core/Application.h" 
// ---

//Platform
#include "Comphi/Platform/IFileRef.h"
#include "Comphi/Platform/IInput.h"

#ifdef CPHI_WINDOWS_PLATFORM
	#include "Comphi/Platform/Windows/FileRef.h"
#endif // WINDOWS
// ---
 
//API
#include "Comphi/API/ComphiAPI.h"
// ---

// ------------ Comphi Entry Point ------------
#include "Comphi/Core/EntryPoint.h"
// --------------------------------------------