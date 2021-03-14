#include "cphipch.h"
#include "GraphicsAPI.h"

namespace Comphi {
    
    IShaderPipeline* GraphicsAPI::create::ShaderPipeline()
    {
        return new OpenGL::ShaderPipeline();
    }

    IVertexBuffer* GraphicsAPI::create::VertexBuffer(float* vertices, uint count)
    {
        switch (activeAPI)
        {
        case RenderingAPI::OpenGL:
            return new OpenGL::VertexBuffer(vertices, count);
        case RenderingAPI::DirectX:
            return new DirectX::VertexBuffer();
        default:
            COMPHILOG_CORE_ERROR("No rendering API Selected!");
            break;
        }
        return nullptr;
    }

    IIndexBuffer* GraphicsAPI::create::IndexBuffer(uint* indices)
    {
        switch (activeAPI)
        {
        case RenderingAPI::OpenGL:
            return new OpenGL::IndexBuffer(indices);
        case RenderingAPI::DirectX:
            return new DirectX::IndexBuffer();
        default:
            COMPHILOG_CORE_ERROR("No rendering API Selected!");
            break;
        }
        return nullptr;
    }

    IShaderProgram* GraphicsAPI::create::ShaderProgram(Comphi::ShaderType shaderType, IFileRef* shaderFile)
    {
        switch (activeAPI)
        {
        case RenderingAPI::OpenGL:
            return new OpenGL::ShaderProgram(shaderType,shaderFile);
        case RenderingAPI::DirectX:
        default:
            COMPHILOG_CORE_ERROR("No rendering API Selected!");
            break;
        }
        return nullptr;
    }

}
