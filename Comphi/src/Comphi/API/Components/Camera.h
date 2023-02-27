#pragma once
#include "Component.h"
#include "Comphi/Renderer/ICamera.h"

namespace Comphi {

	//TODO: Implement Stereoscopic Camera & Ortographic?
	class Camera : public Component, public ICamera
	{
	public:
		Camera() = default;
		virtual void cleanUp() override {};
		virtual glm::mat4 getProjectionMatrix() override { return ((ICamera*)this)->getProjectionMatrix(); };
	};

	typedef std::shared_ptr<Camera> CameraPtr;

}
