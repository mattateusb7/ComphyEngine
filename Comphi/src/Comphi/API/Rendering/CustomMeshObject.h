#pragma once
#include "Comphi/Utils/ModelLoader.h"
#include "Comphi/API/Rendering/ShaderBufferData.h"

namespace Comphi {

	template<typename vx = Vertex, typename ix = Index>
	struct CustomMeshDataBuffers {
		std::vector<vx> vertexDataArray;
		std::vector<ix> indexDataArray;
		ShaderBufferDataPtr vertexBuffer;
		ShaderBufferDataPtr indexBuffer;
	};

	template<typename vx = Vertex, typename ix = Index>
	class CustomMeshObject : IObject 
	{
	public:
		//Custom data VertexAttribute Desctiption
		inline CustomMeshObject(CustomMeshDataBuffers<vx, ix> customMeshDataBuffers);
		CustomMeshDataBuffers<vx, ix> meshDataBuffers;

		virtual void cleanUp() override {};
		typedef std::shared_ptr<CustomMeshObject<vx, ix>> Ptr;
	};

}