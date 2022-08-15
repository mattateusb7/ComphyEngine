#pragma once
#include "Comphi/Renderer/IObjects/IVertexBuffer.h"
#include "Comphi/Renderer/Vulkan/Objects/MemBuffer.h"
#include <glm/glm.hpp>
#include <vulkan/vulkan_core.h>

namespace Comphi::Vulkan {

	struct Vertex {
		glm::vec2 pos;
		glm::vec3 color;
	};

	class VertexBuffer : public IVertexBuffer
	{
	public:

		VertexBuffer(const std::vector<Vertex>& vertices, const MemBuffer::GraphicsHandler& graphicsHandler);
		uint32_t vertexCount;
		std::unique_ptr<MemBuffer> buffer;

		static VkVertexInputBindingDescription getBindingDescription() {
			VkVertexInputBindingDescription bindingDescription{};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(Vertex);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX; //VK_VERTEX_INPUT_RATE_INSTANCE for instanced rendering

			return bindingDescription;
		}

		static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
			std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(Vertex, pos);

			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[1].offset = offsetof(Vertex, color);

			return attributeDescriptions;
		}

		virtual void bind(VkCommandBuffer& commandBuffer);
		virtual void bind() override;
		virtual void unbind() override;
	};
}

