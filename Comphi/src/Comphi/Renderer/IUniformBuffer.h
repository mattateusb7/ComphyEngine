#pragma once

namespace Comphi {

	struct UniformBufferObject {
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 proj;
	};

	class IUniformBuffer : public UniformBufferObject
	{
	public:
		void operator= (const UniformBufferObject& other) 
		{
			this->model = other.model;
			this->view  = other.view;
			this->proj  = other.proj;
		};
	};
}