#pragma once
#include "GameObject.h"

namespace Comphi {

#define CameraInstance std::shared_ptr<Camera>
#define MakeCameraInstance std::make_shared<Camera>

	struct CameraProperties {
		float FOV = 45.0f;
		float NearPlane = 0.1f;
		float FarPlane = 10.0f;
	};

	class Camera : public SceneObjectData
	{
	public:
		Camera(CameraProperties cameraProperties = {}, SceneObjectData sceneData = {});

		CameraProperties cameraProperties;
		glm::mat4 getViewMatrix();
		glm::mat4 getProjectionMatrix();

	};
}
