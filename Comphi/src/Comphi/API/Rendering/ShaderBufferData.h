#pragma once
#include "Comphi/Renderer/IUniformBuffer.h"

namespace Comphi {
	
	/*class ShaderBufferData : public IUniformBuffer
	{
	public:
		ShaderBufferData()  = default;
		~ShaderBufferData() = default;
	};*/

	typedef std::shared_ptr<IUniformBuffer> ShaderBufferDataPtr;
	typedef std::vector<ShaderBufferDataPtr> ShaderBuffers;


}