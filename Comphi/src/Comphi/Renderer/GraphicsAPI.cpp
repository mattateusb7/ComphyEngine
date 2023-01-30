#include "cphipch.h"
#include "GraphicsAPI.h"

#include "Comphi/Renderer/Vulkan/GraphicsContext.h"
#include "Comphi/Renderer/Vulkan/Objects/ShaderProgram.h"
#include "Comphi/Renderer/Vulkan/Objects/ImageView.h"

namespace Comphi {
   
    IGraphicsContext* GraphicsAPI::create::GraphicsContext(void* windowHandler)
    {
        switch (activeAPI)
        {
        case RenderingAPI::Vulkan:
            currentGraphicsContext = std::make_shared<Vulkan::GraphicsContext>(Vulkan::GraphicsContext(*static_cast<GLFWwindow*>(windowHandler)));
            return currentGraphicsContext.get();
        default:
            COMPHILOG_CORE_FATAL("No rendering API Selected!");
            break;
        }
        return nullptr;
    }

    CameraInstance GraphicsAPI::create::Camera(CameraProperties cameraProperties, TransformData transformData)
    {
        //TODO : Switch RenderingAPI
        return std::make_shared<Comphi::Camera>(cameraProperties,transformData);
    }

    GameObjectInstance GraphicsAPI::create::GameObject(MeshData meshData, TransformData transformData)
    {
        //TODO : Switch RenderingAPI
        return std::make_shared<Comphi::GameObject>(meshData, transformData);
    }

    SceneInstance GraphicsAPI::create::Scene()
    {
        //TODO : Switch RenderingAPI
        return std::make_shared<Comphi::Scene>();
    }

    ShaderInstance GraphicsAPI::create::ShaderProgram(Comphi::ShaderType shaderType, IFileRef& shaderFile)
    {
        //TODO : Switch RenderingAPI
        return std::make_shared<Vulkan::ShaderProgram>(shaderType, shaderFile);
    }

    TextureInstance GraphicsAPI::create::Texture(IFileRef& fileref)
    {
        //TODO : Switch RenderingAPI
        std::shared_ptr<Vulkan::ImageView> texture = std::make_shared<Vulkan::ImageView>();
        texture->initTextureImageView(fileref);
        return texture;
    }

    MaterialInstance GraphicsAPI::create::Material(MaterialProperties materialProperties)
    {
        //TODO : Switch RenderingAPI
        return std::make_shared<Vulkan::Material>(materialProperties);
    }

    MeshInstance GraphicsAPI::create::Mesh(IFileRef& objFile, MaterialInstance& material)
    {
        //TODO : Switch RenderingAPI
        return std::make_shared<Vulkan::MeshObject>(objFile, *static_cast<Vulkan::Material*>(material.get()));
    }

    MeshInstance GraphicsAPI::create::Mesh(VertexArray& vertices, IndexArray& indices, MaterialInstance& material)
    {
        //TODO : Switch RenderingAPI
        return std::make_shared<Vulkan::MeshObject>(vertices, indices, *static_cast<Vulkan::Material*>(material.get()));
    }
   
    void GraphicsAPI::destroyObject(IObject* inst)
    {
        inst->cleanUp();
    }

}
