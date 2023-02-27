#pragma once

#include <stdio.h>
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm> // Necessary for std::clamp
#include <functional>
#include <chrono>
#include <cstdint> // Necessary for uint32_t
#include <limits> // Necessary for std::numeric_limits

#include <string>
#include <fstream>
#include <streambuf>
#include <sstream>

#include <vector>
#include <stack>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/hash.hpp>

#include "Comphi/Core/Core.h"
#include "Comphi/Core/Log.h"

#ifdef CPHI_WINDOWS_PLATFORM 
	#include <Windows.h>
#endif