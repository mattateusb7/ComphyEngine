#include "cphipch.h"
#include "GraphicsAPI.h"

namespace Comphi {
    
    IShaderPipeline* GraphicsAPI::create::ShaderPipeline()
    {
        return new OpenGL::ShaderPipeline();
    }

    IVertexBuffer* GraphicsAPI::create::VertexBuffer(const float& vertices, const uint& count)
    {
        switch (activeAPI)
        {
        case RenderingAPI::OpenGL:
            return new OpenGL::VertexBuffer(vertices, count);
        case RenderingAPI::Vulkan:
            COMPHILOG_CORE_ERROR("Not Implemented!");
            break;
        default:
            COMPHILOG_CORE_FATAL("No rendering API Selected!");
            break;
        }
        return nullptr;
    }

    IIndexBuffer* GraphicsAPI::create::IndexBuffer(const uint& indices)
    {
        switch (activeAPI)
        {
        case RenderingAPI::OpenGL:
            return new OpenGL::IndexBuffer(indices);
        case RenderingAPI::Vulkan:
            COMPHILOG_CORE_ERROR("Not Implemented!");
            break;
        default:
            COMPHILOG_CORE_FATAL("No rendering API Selected!");
            break;
        }
        return nullptr;
    }

    IShaderProgram* GraphicsAPI::create::ShaderProgram(Comphi::ShaderType shaderType, IFileRef& shaderFile)
    {
        switch (activeAPI)
        {
        case RenderingAPI::OpenGL:
            return new OpenGL::ShaderProgram(shaderType,shaderFile);
        case RenderingAPI::Vulkan:
            COMPHILOG_CORE_ERROR("Not Implemented!");
            break;
        default:
            COMPHILOG_CORE_FATAL("No rendering API Selected!");
            break;
        }
        return nullptr;
    }

    IGraphicsContext* GraphicsAPI::create::GraphicsContext(GLFWwindow& windowHandler)
    {
        switch (activeAPI)
        {
        case RenderingAPI::OpenGL:
            return new OpenGL::GraphicsContext(windowHandler);
        case RenderingAPI::Vulkan:
            return new Vulkan::GraphicsContext(windowHandler);
        default:
            COMPHILOG_CORE_FATAL("No rendering API Selected!");
            break;
        }
        return nullptr;
    }

}
