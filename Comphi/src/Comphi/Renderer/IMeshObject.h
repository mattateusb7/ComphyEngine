#pragma once
#include "Comphi/Allocation/IObject.h"
#include "Comphi/Platform/IFileRef.h"
#include "Comphi/Renderer/IMaterial.h"
#include "Comphi/Renderer/IVertexBuffer.h"
#include "Comphi/Renderer/IIndexBuffer.h"

namespace Comphi {

	class IMeshObject : public IObject //TODO:rename to IMesh or IMeshOBJ
	{
	public:
		IFileRef* i_ModelFileOBJ;
		std::shared_ptr<IMaterial> i_material; //TODO: Add Material-Attributes for vertexGroups
		std::shared_ptr<IVertexBuffer> i_vertices;
		std::shared_ptr<IIndexBuffer> i_indices;
	};
}

