#include "cphipch.h"
#include "GameObject.h"

namespace Comphi {
	
	GameObject::GameObject(MeshInstance mesh, SceneObjectData sceneData)
	{
		transform = sceneData.transform;
		parent = sceneData.parent;

		this->mesh = mesh;
		initializeMeshData();
	}


	void GameObject::initializeMeshData() {
		if (mesh.get() && mesh->material.get()) {
			sendDataLayoutToDesciptorPool();
		}
	}

	void GameObject::sendDataLayoutToDesciptorPool()
	{
		initMVP();
		//TODO: Add DescriptoSetLayoutProperties Struct in the future to allow diferent layouts compatible with Descriptor Pool
		mesh->material->graphicsPipeline.sendDescriptorSet(mesh->material->shaderTextures, MVP_UBOs);
	}

	void GameObject::initMVP()
	{
		int MAX_FRAMES_IN_FLIGHT = *Vulkan::GraphicsHandler::get()->MAX_FRAMES_IN_FLIGHT;
		MVP_UBOs.resize(MAX_FRAMES_IN_FLIGHT);
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			//initialize
			UniformBufferObject ubo = {};
			MVP_UBOs[i] = UniformBufferObject(ubo);
		}
	}

	void GameObject::updateMVP(UniformBufferObject mvpUBO, uint currentImage)
	{
		//Maybe Send Camera into update 
		//circular dependency
		void* data;
		vkMapMemory(*Vulkan::GraphicsHandler::get()->logicalDevice, MVP_UBOs[currentImage].bufferMemory, 0, sizeof(mvpUBO), 0, &data);
		memcpy(data, &mvpUBO, sizeof(mvpUBO));
		vkUnmapMemory(*Vulkan::GraphicsHandler::get()->logicalDevice, MVP_UBOs[currentImage].bufferMemory);
	}

}