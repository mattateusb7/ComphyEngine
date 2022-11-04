#include "cphipch.h"
#include "GraphicsAPI.h"

namespace Comphi {
    
    /*IGraphicsPipeline* GraphicsAPI::create::GraphicsPipeline()
    {
        switch (activeAPI)
        {
        case RenderingAPI::Vulkan: 
        {
            //Vulkan::GraphicsPipeline::GraphicsPipelineSetupData data {};
            //data.viewport = {};
            //data.scissor = {};
            //return new Vulkan::GraphicsPipeline();
            break;
        }
        default:
            COMPHILOG_CORE_FATAL("No rendering API Selected!");
            break;
        }
        return nullptr;
    }

    IVertexBuffer* GraphicsAPI::create::VertexBuffer(IGraphicsContext* currentGraphicsContext, const VertexArray& vertices)
    {
        switch (activeAPI)
        {
        case RenderingAPI::Vulkan: 
        {
            //auto graphicsContext = static_cast<Vulkan::GraphicsContext*>(currentGraphicsContext);
            //return new Vulkan::VertexBuffer(vertices, graphicsContext->getGraphicsHandler());
            break;
        }
        default:
            COMPHILOG_CORE_FATAL("No rendering API Selected!");
            break;
        }
        return nullptr;
    }

    IIndexBuffer* GraphicsAPI::create::IndexBuffer(IGraphicsContext* currentGraphicsContext, const IndexArray& indices)
    {
        switch (activeAPI)
        {
        case RenderingAPI::Vulkan:
        {
            //auto graphicsContext = static_cast<Vulkan::GraphicsContext*>(currentGraphicsContext);
            //return new Vulkan::IndexBuffer(indices, graphicsContext->getGraphicsHandler());
            break;
        }
        default:
            COMPHILOG_CORE_FATAL("No rendering API Selected!");
            break;
        }
        return nullptr;
    }

    IShaderProgram* GraphicsAPI::create::ShaderProgram(IGraphicsContext* currentGraphicsContext, Comphi::ShaderType shaderType, IFileRef& shaderFile)
    {
        switch (activeAPI)
        {
        case RenderingAPI::Vulkan:
        {
            //auto graphicsContext = static_cast<Vulkan::GraphicsContext*>(currentGraphicsContext);
            //return new Vulkan::ShaderProgram(shaderType, shaderFile, *graphicsContext->getGraphicsHandler()->logicalDevice.get());
            break;
        }
        default:
            COMPHILOG_CORE_FATAL("No rendering API Selected!");
            break;
        }
        return nullptr;
    }
    */

    IGraphicsContext* GraphicsAPI::create::GraphicsContext(GLFWwindow& windowHandler)
    {
        switch (activeAPI)
        {
        case RenderingAPI::Vulkan:
            return new Vulkan::GraphicsContext(windowHandler);
        default:
            COMPHILOG_CORE_FATAL("No rendering API Selected!");
            break;
        }
        return nullptr;
    }

}
