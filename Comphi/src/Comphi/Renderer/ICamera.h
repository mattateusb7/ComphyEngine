#pragma once

namespace Comphi {
	
	struct CameraProperties {
		float FOV = 45.0f;
		float NearPlane = 0.1f;
		float FarPlane = 10.0f;
	};

	class ICamera
	{
	public:
		CameraProperties properties;
		//glm::mat4 getProjectionMatrix() {
		//	glm::mat4 projectionMatrix = glm::perspective(
		//		glm::radians(FOV),
		//		(float)Vulkan::GraphicsHandler::get()->swapChainExtent->width / Vulkan::GraphicsHandler::get()->swapChainExtent->height,
		//		NearPlane, FarPlane);
		//	projectionMatrix[1][1] *= -1;
		//
		//	return projectionMatrix;
		//}
	};
}