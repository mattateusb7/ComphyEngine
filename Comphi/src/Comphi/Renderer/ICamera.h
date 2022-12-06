#pragma once
#include "glm/mat4x4.hpp"
namespace Comphi {

	class ICamera 
	{
	public:
		virtual glm::mat4 getViewMatrix() = 0;
		virtual glm::mat4 getProjectionMatrix() = 0;
	};
}