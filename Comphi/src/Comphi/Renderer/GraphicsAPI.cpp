#include "cphipch.h"
#include "GraphicsAPI.h"

#include "Comphi/Renderer/Vulkan/GraphicsContext.h"
#include "Comphi/Renderer/Vulkan/Objects/ShaderProgram.h"
#include "Comphi/Renderer/Vulkan/Objects/ImageView.h"

namespace Comphi {

    IObjectPool GraphicsAPI::cameraPool = IObjectPool();
    IObjectPool GraphicsAPI::gameObjectPool = IObjectPool();
    IObjectPool GraphicsAPI::shaderPool = IObjectPool();
    IObjectPool GraphicsAPI::texturePool = IObjectPool();
    IObjectPool GraphicsAPI::materialPool = IObjectPool();
    IObjectPool GraphicsAPI::meshPool = IObjectPool();
   
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

    CameraInstance GraphicsAPI::create::Camera(CameraProperties cameraProperties, TransformData transformData, IObjectPool* pool)
    {
        //TODO : Switch RenderingAPI
        std::shared_ptr<Comphi::Camera> camera = std::make_shared<Comphi::Camera>(cameraProperties, transformData);
        pool->Add(camera.get());
        return camera;
    }

    GameObjectInstance GraphicsAPI::create::GameObject(MeshData meshData, TransformData transformData, IObjectPool* pool)
    {
        //TODO : Switch RenderingAPI
        std::shared_ptr<Comphi::GameObject> gameObject = std::make_shared<Comphi::GameObject>(meshData, transformData);
        pool->Add(gameObject.get());
        return gameObject;
    }

    SceneInstance GraphicsAPI::create::Scene()
    {
        //TODO : Switch RenderingAPI
        return std::make_shared<Comphi::Scene>();
    }

    ShaderInstance GraphicsAPI::create::ShaderProgram(Comphi::ShaderType shaderType, IFileRef& shaderFile, IObjectPool* pool)
    {
        //TODO : Switch RenderingAPI
        std::shared_ptr<Vulkan::ShaderProgram> shader = std::make_shared<Vulkan::ShaderProgram>(shaderType, shaderFile);
        pool->Add(shader.get());
        return shader;
    }

    TextureInstance GraphicsAPI::create::Texture(IFileRef& fileref, IObjectPool* pool)
    {
        //TODO : Switch RenderingAPI
        std::shared_ptr<Vulkan::ImageView> texture = std::make_shared<Vulkan::ImageView>();
        texture->initTextureImageView(fileref);
        pool->Add(texture.get());
        return texture;
    }

    MaterialInstance GraphicsAPI::create::Material(MaterialResources materialProperties, IObjectPool* pool)
    {
        //TODO : Switch RenderingAPI
        std::shared_ptr<Vulkan::Material> material = std::make_shared<Vulkan::Material>(materialProperties);
        pool->Add(material.get());
        return material;
    }

    MeshInstance GraphicsAPI::create::Mesh(IFileRef& objFile, MaterialInstance& material, IObjectPool* pool)
    {
        //TODO : Switch RenderingAPI
        std::shared_ptr<Vulkan::MeshObject> mesh = std::make_shared<Vulkan::MeshObject>(objFile, *static_cast<Vulkan::Material*>(material.get()));
        pool->Add(mesh.get());
        return mesh;
    }

    MeshInstance GraphicsAPI::create::Mesh(VertexArray& vertices, IndexArray& indices, MaterialInstance& material, IObjectPool* pool)
    {
        //TODO : Switch RenderingAPI
        std::shared_ptr<Vulkan::MeshObject> mesh = std::make_shared<Vulkan::MeshObject>(vertices, indices, *static_cast<Vulkan::Material*>(material.get()));
        pool->Add(mesh.get());
        return mesh;
    }

    void GraphicsAPI::cleanUpInstances()
    {
        COMPHILOG_CORE_TRACE("Cleaning ComphiAPI Instances...");
        cameraPool     .cleanUp();
        gameObjectPool .cleanUp();
        shaderPool     .cleanUp();
        texturePool    .cleanUp();
        materialPool   .cleanUp();
        meshPool       .cleanUp();
        COMPHILOG_CORE_TRACE("Finished cleaning ComphiAPI Instances !");
    }

}
