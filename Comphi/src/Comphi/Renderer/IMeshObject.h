#pragma once
#include "Comphi/Platform/IFileRef.h"
#include "Comphi/Renderer/IMaterial.h"
#include "Comphi/Renderer/IVertexBuffer.h"
#include "Comphi/Renderer/IIndexBuffer.h"
#include "Comphi/Renderer/IUniformBuffer.h"

namespace Comphi {

	class IMeshObject
	{
	public:
		IFileRef* i_objFile;
		std::shared_ptr<IMaterial> i_material; //TODO: Add Material-Attributes for vertexGroups
		std::shared_ptr<IVertexBuffer> i_vertices;
		std::shared_ptr<IIndexBuffer> i_indices;

		virtual void initMVP() = 0;
		virtual void updateMVP(uint currentImage) = 0;
		std::vector<IUniformBuffer> i_MVP_UBOs;

		//Rendering
		virtual void bind(void* commandBuffer) = 0;

	protected :
		IMeshObject() = default;
	};
}

