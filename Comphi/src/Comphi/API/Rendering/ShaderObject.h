#pragma once
#include "Comphi/Renderer/IShaderProgram.h"

namespace Comphi {

	/*class ShaderObject : public IShaderProgram
	{
	public:
		ShaderObject()  = default;
		~ShaderObject() = default;
	};*/

	typedef std::shared_ptr<IShaderProgram> ShaderObjectPtr;
	typedef std::vector<ShaderObjectPtr> ShaderPrograms;

}