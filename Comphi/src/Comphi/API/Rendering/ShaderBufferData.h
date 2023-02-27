#pragma once
#include "Comphi/Renderer/IUniformBuffer.h"

namespace Comphi {
	
	/*class ShaderBufferData : public IUniformBuffer
	{
	public:
		virtual void cleanUp() override {};
		virtual void updateBufferData(const void* dataArray) override {};
		ShaderBufferData()  = default;
		~ShaderBufferData() = default;
	};*/

	typedef std::shared_ptr<IUniformBuffer> ShaderBufferDataPtr;
	typedef std::vector<ShaderBufferDataPtr> ShaderBuffers;


}