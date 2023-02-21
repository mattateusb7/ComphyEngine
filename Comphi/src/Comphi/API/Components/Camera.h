#pragma once
#include "Component.h"
#include "Comphi/Renderer/ICamera.h"

namespace Comphi {

	//TODO: Implement Stereoscopic Camera.
	class Camera : public Component, public ICamera
	{
	public:
		Camera(CameraProperties cameraProperties = {});
	};

	typedef std::shared_ptr<Camera> CameraPtr;

}
