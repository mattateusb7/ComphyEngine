#pragma once

#include "Comphi/Platform/IFileRef.h"
#include "Comphi/Renderer/Vulkan/Objects/VertexBuffer.h"
#include "Comphi/Renderer/Vulkan/Objects/IndexBuffer.h"
#include "Comphi/Core/API/Material.h"

namespace Comphi { //TODO: Comphi namepsace objects should use platform & renderer independent interfaces (API)
	class MeshObject //Add to GameObject (if not Empty)
	{
	public:
		MeshObject() = default;

		//TODO: move material out of MeshObject Contructor to GameObject (that holds both mesh & textures 
		//although each mesh can only render one material at the time... so batched rendering would require us to group all meshes with the same material in a single buffer
		// so its probably not that bad to keep material Refs inside meshes.

		MeshObject(IFileRef& objFile, Material& material);
		void initialize(IFileRef& objFile, Material& material);
		MeshObject(VertexArray& vertices, IndexArray& indices, Material& material);
		void initialize(VertexArray& vertices, IndexArray& indices, Material& material);
		IFileRef* objFile;
		/*
		* Driver developers recommend that you also store multiple buffers, like the vertex and index buffer, into a single VkBuffer
		* (DrawBuffer or maybe batchDrawBuffer/multipleObjs)
		* and use offsets in commands like vkCmdBindVertexBuffers.
		* The advantage is that your data is more cache friendly in that case, because it's closer together.
		*/

		std::shared_ptr<Vulkan::VertexBuffer> vertices;
		std::shared_ptr<Vulkan::IndexBuffer> indices;
		std::shared_ptr<Material> material; //TODO: Add Material-Attributes for vertexGroups

		void sendDataLayoutToDesciptorPool();
		//TODO : Move to TransformObj 
		//Figure out where to initialize & update 
		//Make UniformBuffer Interface (API)
		std::vector<Vulkan::UniformBuffer> MVP_UBOs; //TODO : MVP matrix uniform -> Move to TransformObj

	protected:
		void ParseObjFile(IFileRef& objFile);
	};

	#define MeshInstance std::shared_ptr<MeshObject>
	#define MakeMeshInstance std::make_shared<MeshObject>
}

