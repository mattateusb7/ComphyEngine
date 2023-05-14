#include "cphipch.h"
#include "Camera.h"

namespace Comphi::Vulkan {

	glm::mat4 Camera::getProjectionMatrix()
	{
		float aspect = (float)GraphicsHandler::get()->swapChainExtent->width / GraphicsHandler::get()->swapChainExtent->height;
		float fov_x = atan(tan(properties.FOV / 2) * aspect) * 2;
		glm::mat4 projectionMatrix = glm::perspective(
			glm::radians(properties.FOV),
			aspect,
			properties.NearPlane, properties.FarPlane);
		projectionMatrix[1][1] *= -1;

		//glm::mat4 projectionMatrix1 = glm::ortho(
		//	GraphicsHandler::get()->swapChainExtent->width / 2,
		//	GraphicsHandler::get()->swapChainExtent->width / 2,
		//	GraphicsHandler::get()->swapChainExtent->height / 2,
		//	GraphicsHandler::get()->swapChainExtent->height / 2);
		//projectionMatrix1[1][1] *= -1;

		return projectionMatrix;
	}

}