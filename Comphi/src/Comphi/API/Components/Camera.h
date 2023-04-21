#pragma once
#include "Component.h"
#include "Comphi/Renderer/ICamera.h"

namespace Comphi {

	//TODO: Implement Stereoscopic Camera & Ortographic?
	class Camera : public Component, public ICamera
	{
	public:
		Camera(ICameraPtr& iCameraPtr) : ICamera(*iCameraPtr.get()) {
			this->iCameraPtr = iCameraPtr;
		}
		virtual void cleanUp() override {};
		virtual glm::mat4 getProjectionMatrix() override { return iCameraPtr->getProjectionMatrix(); };
	private:
		ICameraPtr iCameraPtr;
	};

	typedef std::shared_ptr<Camera> CameraPtr;

}
