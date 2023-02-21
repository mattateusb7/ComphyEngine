#pragma once
#include "Comphi/Allocation/IObject.h"

namespace Comphi {

	//Note: more structs can be defined by Client and passed using template initialization of shaderBufferData objects!

	struct MVMatrixObject {
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
	};
	struct PMatrixObject {
		alignas(16) glm::mat4 proj;
	};

	struct MVPMatrixObject { 
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 proj;
	};

	enum BufferUsage {
		UniformBuffer,
		VertexBuffer,
		IndexBuffer
	};

	class IUniformBuffer : public IObject
	{
	public:
		virtual void updateBufferData(const void* dataArray) = 0;
	};
}