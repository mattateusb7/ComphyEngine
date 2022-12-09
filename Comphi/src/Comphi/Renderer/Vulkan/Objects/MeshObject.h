#pragma once

#include "Comphi/Platform/IFileRef.h"
#include "Comphi/Renderer/IMeshObject.h"
#include "Comphi/Renderer/Vulkan/Objects/VertexBuffer.h"
#include "Comphi/Renderer/Vulkan/Objects/IndexBuffer.h"
#include "Comphi/Renderer/Vulkan/Objects/Material.h"

namespace Comphi::Vulkan {

	class MeshObject : public IMeshObject
	{
	public:
		//TODO: move material out of MeshObject Contructor to GameObject (that holds both mesh & textures 
		//although each mesh can only render one material at the time... so batched rendering would require us to group all meshes with the same material in a single buffer
		// so its probably not that bad to keep material Refs inside meshes.

		MeshObject(IFileRef& objFile, Material& material);
		MeshObject(VertexArray& vertices, IndexArray& indices, Material& material);

		//Rendering
		/*
		* Driver developers recommend that you also store multiple buffers, like the vertex and index buffer, into a single VkBuffer
		* (DrawBuffer or maybe batchDrawBuffer/multipleObjs)
		* and use offsets in commands like vkCmdBindVertexBuffers.
		* The advantage is that your data is more cache friendly in that case, because it's closer together.
		*/
		std::vector<UniformBuffer> MVP_UBOs;

		void bind(void* commandBuffer);
		void updateMVP(UniformBufferObject& mvpUBO, uint currentImage);

	protected:
		void ParseObjFile(IFileRef& objFile);
		void initializeMeshData();
		void initializeMVPMatrices();
	};

}

