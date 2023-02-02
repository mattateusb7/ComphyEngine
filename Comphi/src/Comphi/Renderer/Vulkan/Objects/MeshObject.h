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
		//TODO: we are keeping material Refs inside meshes, maybe move material out of MeshObject 
		//one mesh can render moree than one material at the same time... maybe make it a vector of indexed materials bound to indices
		//don't forget batched rendering would require us to group all meshes with the same material in a single buffer

		MeshObject(IFileRef& objFile, Material& material);
		MeshObject(VertexArray& vertices, IndexArray& indices, Material& material);

		//Rendering
		/* TODO: 
		* Driver developers recommend storeing multiple buffers, like the vertex and index buffer, into a single VkBuffer
		* (DrawBuffer or maybe batchDrawBuffer/multipleObjs)
		* and use offsets in commands like vkCmdBindVertexBuffers.
		* The advantage is that your data is more cache friendly in that case, because it's closer together.
		*/
		std::vector<UniformBuffer> MVP_UBOs;

		void bind(void* commandBuffer);
		void updateMVP(UniformBufferObject& mvpUBO, uint currentImage);

		virtual void cleanUp() override {}; //TODO : Implement
	protected:
		void ParseObjFile(IFileRef& objFile);
		void initializeMeshData();
		void initializeMVPMatrices();
	};

}

