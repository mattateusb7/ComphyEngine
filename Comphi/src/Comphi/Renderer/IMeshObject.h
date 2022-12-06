#pragma once
#include "Comphi/Platform/IFileRef.h"
#include "Comphi/Renderer/IMaterial.h"
#include "Comphi/Renderer/IVertexBuffer.h"
#include "Comphi/Renderer/IIndexBuffer.h"
#include "Comphi/Renderer/IUniformBuffer.h"

namespace Comphi {

	class IMeshObject //TODO:rename to IMesh or IMeshOBJ
	{
	public:
		IFileRef* i_ModelFileOBJ;
		std::shared_ptr<IMaterial> i_material; //TODO: Add Material-Attributes for vertexGroups
		std::shared_ptr<IVertexBuffer> i_vertices;
		std::shared_ptr<IIndexBuffer> i_indices;
		std::vector<IUniformBuffer> i_MVP_UBOs;
	protected:
		void initializeMeshData() {
			if (i_material.get() != nullptr) {
				//send Data Layout To DesciptorPool
				//TODO: Add DescriptoSetLayoutProperties Struct in the future to allow diferent layouts compatible with Descriptor Pool
				i_material->sendDescriptorSet(i_MVP_UBOs);
			}
		};
	};
}

