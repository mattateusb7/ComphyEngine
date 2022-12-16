#pragma once
#include "GameObject.h"

namespace Comphi {

	struct CameraProperties {
		float FOV = 45.0f;
		float NearPlane = 0.1f;
		float FarPlane = 10.0f;
	};

	//TODO: Implement Stereoscopic Camera.
	class Camera : public GameObject, public CameraProperties
	{
	public:
		Camera(CameraProperties cameraProperties = {}, TransformData transformData = {});

		//Camera Action
		ActionHandle action;

		glm::mat4 getViewMatrix();
		glm::mat4 getProjectionMatrix();

	};

	typedef std::shared_ptr<Camera> CameraInstance;

}
