#pragma once
#include "Comphi/Allocation/IObject.h"

namespace Comphi {

	//we probably wont be exposing uniform buffers to the engineAPI...
	enum BufferUsage {
		UniformBuffer,
		VertexBuffer,
		IndexBuffer,
		DrawIndirect,
		BufferStorageDynamic
	};

	class IUniformBuffer : public IObject
	{
	public:
		BufferUsage bufferUsage;
		virtual void updateBufferData(const void* dataArray) = 0;
	};
}