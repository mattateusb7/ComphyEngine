#include "cphipch.h"
#include "Camera.h"
#include "Comphi/Renderer/Vulkan/GraphicsHandler.h"

namespace Comphi {

	Camera::Camera(CameraProperties cameraProperties, GameObjectData sceneData)
	{
		parent = sceneData.parent;
		transform = sceneData.transform;
		this->cameraProperties = cameraProperties; 
	}

	glm::mat4 Camera::getViewMatrix() 
	{
		return glm::lookAt(transform.position, transform.getLookVector(), transform.getUpVector());
	}

	glm::mat4 Camera::getProjectionMatrix() 
	{
		glm::mat4 projectionMatrix = glm::perspective(
			glm::radians(cameraProperties.FOV),
			(float)Vulkan::GraphicsHandler::get()->swapChainExtent->width / Vulkan::GraphicsHandler::get()->swapChainExtent->height,
			cameraProperties.NearPlane, cameraProperties.FarPlane);
		projectionMatrix[1][1] *= -1;

		return projectionMatrix;
	}
}
