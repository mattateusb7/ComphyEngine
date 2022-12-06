#include "cphipch.h"
#include "Camera.h"
#include "Comphi/Renderer/Vulkan/GraphicsHandler.h" //TODO: REMOVE THIS

namespace Comphi {

	Camera::Camera(CameraProperties cameraProperties, TransformData transformData)
	{
		(TransformData)*this = transformData;
		(CameraProperties)*this = cameraProperties;
	}

	glm::mat4 Camera::getViewMatrix() //TODO: << implement ICamera Abstraction
	{
		return glm::lookAt(transform.position, transform.getLookVector(), transform.getUpVector());
	}

	glm::mat4 Camera::getProjectionMatrix()  //TODO: << implement ICamera Abstraction
	{
		glm::mat4 projectionMatrix = glm::perspective(
			glm::radians(FOV),
			(float)Vulkan::GraphicsHandler::get()->swapChainExtent->width / Vulkan::GraphicsHandler::get()->swapChainExtent->height,
			NearPlane, FarPlane);
		projectionMatrix[1][1] *= -1;

		return projectionMatrix;
	}
}
