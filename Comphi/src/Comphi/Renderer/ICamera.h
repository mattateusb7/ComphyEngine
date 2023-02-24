#pragma once

namespace Comphi {
	
	struct CameraProperties {
		float FOV = 45.0f;
		float NearPlane = 0.1f;
		float FarPlane = 10.0f;
	};

	class ICamera
	{
	public:
		CameraProperties properties;
		virtual glm::mat4 getProjectionMatrix() = 0;
	};
}