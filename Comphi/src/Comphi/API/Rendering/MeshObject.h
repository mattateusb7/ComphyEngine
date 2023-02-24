#pragma once
#include "Comphi/Utils/ModelLoader.h"
#include "Comphi/API/Rendering/ShaderBufferData.h"

namespace Comphi {

	struct MeshBuffers{
		ShaderBufferDataPtr vertexBuffer;
		ShaderBufferDataPtr indexBuffer;
	};

	class MeshObject : public IObject
	{
	public:
		//Default VertexAttribute Desctiption
		MeshObject(IFileRef& modelFile, MeshBuffers& meshBuffers);
		MeshObject(MeshData& meshData, MeshBuffers& meshBuffers);
		MeshObject(VertexArray& vertexData, IndexArray& indexData, MeshBuffers& meshBuffers);

		static IndexArray& fillEmptyIndexArray(VertexArray& vertexData, IndexArray& indexData);

		MeshData meshData;
		MeshBuffers meshBuffers;
		virtual void cleanUp() override {};

	private:
		void initMeshBuffers(MeshBuffers& meshBuffers);
	};

	typedef std::shared_ptr<MeshObject> MeshObjectPtr;

}


