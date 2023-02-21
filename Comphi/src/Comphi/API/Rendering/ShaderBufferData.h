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

	typedef std::shared_ptr<IUniformBuffer> UniformBufferDataPtr							   ;
	typedef std::shared_ptr<IUniformBuffer> VertexBufferDataPtr;
	typedef std::shared_ptr<IUniformBuffer> IndexBufferDataPtr ;


	typedef std::vector<ShaderBufferDataPtr> ShaderBuffers;


}