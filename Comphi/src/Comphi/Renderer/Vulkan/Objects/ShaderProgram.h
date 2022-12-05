#pragma once
#include "Comphi/Renderer/Vulkan/GraphicsHandler.h"
#include "Comphi/Renderer/IShaderProgram.h"
#include "Comphi/Platform/IFileRef.h"

namespace Comphi::Vulkan {

	class ShaderProgram : public IShaderProgram
	{
	public:
		ShaderProgram(Comphi::ShaderType shaderType, IFileRef& shaderFile);
		VkShaderModule shaderModule;

		//bool operator==(ShaderProgram& other) {
		//	return other. GetType() == GetType();
		//}
		~ShaderProgram();
	protected:
		void createShaderModule();
	};

	//TODO: make GraphicsAPI Factory

}