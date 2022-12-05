#pragma once
#include "GameObject.h"

namespace Comphi {

#define CameraInstance std::shared_ptr<Comphi::Camera>
#define MakeCameraInstance std::make_shared<Comphi::Camera>

	struct CameraProperties {
		float FOV = 45.0f;
		float NearPlane = 0.1f;
		float FarPlane = 10.0f;
	};

	//TODO: Implement Stereoscopic Camera.
	class Camera : public GameObjectData
	{
	public:
		Camera(CameraProperties cameraProperties = {}, GameObjectData sceneData = {});

		//Camera Action
		ActionHandle action;

		CameraProperties cameraProperties;
		glm::mat4 getViewMatrix();
		glm::mat4 getProjectionMatrix();

	};
}
