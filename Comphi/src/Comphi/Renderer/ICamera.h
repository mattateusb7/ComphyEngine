#pragma once
#include "Comphi/Allocation/IObject.h"

namespace Comphi {
	
	struct CameraProperties {
		float FOV = 45.0f;
		float NearPlane = 0.1f;
		float FarPlane = 10.0f;
	};

	class ICamera : public IObject
	{
	public:
		CameraProperties properties;
		virtual glm::mat4 getProjectionMatrix() = 0;
	};

	typedef std::shared_ptr<ICamera> ICameraPtr;
}