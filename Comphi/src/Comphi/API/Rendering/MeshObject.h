#pragma once
#include "Comphi/Utils/ModelLoader.h"
#include "Comphi/API/Rendering/ShaderBufferData.h"
#include "ShaderBinding.h"

namespace Comphi {

	struct MeshBuffers{
		BufferDataPtr vertexBuffer;
		BufferDataPtr indexBuffer;
	};

	//template<typename vx = Vertex, typename ix = Index>
	class MeshObject : public IObject
	{
	public:
		//Default VertexAttribute Desctiption
		MeshObject(IFileRef& modelFile, MeshBuffers& meshBuffers);
		MeshObject(MeshData& meshData, MeshBuffers& meshBuffers);
		MeshObject(VertexArray& vertexData, IndexArray& indexData, MeshBuffers& meshBuffers);

		MeshData meshData;
		MeshBuffers meshBuffers;
		virtual void cleanUp() override {};

		//typedef std::shared_ptr<MeshObject<vx, ix>> Ptr;

	private:
		static IndexArray& fillEmptyIndexArray(VertexArray& vertexData, IndexArray& indexData);
		void initMeshBuffers(MeshBuffers& meshBuffers);
	};

	typedef std::shared_ptr<MeshObject> MeshObjectPtr;

}

//template<> struct std::hash<Comphi::MeshObjectPtr> {
//	size_t operator()(Comphi::MeshObjectPtr const& mesh) const {
//		return mesh->UID;
//	}
//};
