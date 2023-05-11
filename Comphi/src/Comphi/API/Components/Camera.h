#pragma once
#include "Component.h"
#include "Comphi/Renderer/ICamera.h"
#include "Comphi/API/Rendering/ShaderBufferData.h"

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
		BufferDataPtr bufferViewProjectionMatrix;
	private:
		ICameraPtr iCameraPtr;
	};

	typedef std::shared_ptr<Camera> CameraPtr;

}
