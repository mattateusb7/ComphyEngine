#include "cphipch.h"
#include "ComphiAPI.h"

#include "Comphi/Renderer/Vulkan/GraphicsContext.h"
#include "Comphi/Renderer/Vulkan/Graphics/ShaderProgram.h"
#include "Comphi/Renderer/Vulkan/Graphics/GraphicsPipeline.h"
#include "Comphi/Renderer/Vulkan/Images/ImageView.h"
#include "Comphi/Renderer/Vulkan/Buffers/UniformBuffer.h"

namespace Comphi {

    IObjectPool ComphiAPI::objectPool = IObjectPool();
   
    IGraphicsContext* ComphiAPI::Init::GraphicsContext(void* windowHandler)
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

    ScenePtr ComphiAPI::SceneGraph::Scene()
    {
        ScenePtr scene = std::make_shared<Comphi::Scene>();
        return scene;
    }

    EntityPtr ComphiAPI::SceneGraph::Entity(IObjectPool* pool)
    {
        auto entity = std::make_shared<Comphi::Entity>();
        pool->Add(entity.get());
        return entity;
    }

    CameraPtr ComphiAPI::Components::Camera(CameraProperties cameraProperties, IObjectPool* pool)
    {
        auto camera = std::make_shared<Comphi::Camera>(cameraProperties);
        pool->Add(camera.get());
        return camera;
    }

    TransformPtr ComphiAPI::Components::Transform(IObjectPool* pool)
    {
        auto transform = std::make_shared<Comphi::Transform>();
        pool->Add(transform.get());
        return transform;
    }

    RendererPtr ComphiAPI::Components::Renderer(MaterialInstance& material, MeshData& mesh, IObjectPool* pool)
    {
        auto renderer = std::make_shared<Comphi::Renderer>(material, mesh);
        pool->Add(renderer.get());
        return renderer;
    }

    MaterialPtr ComphiAPI::Rendering::Material(GraphicsPipelineConfiguration config, IObjectPool* pool)
    {
        //Vulkan
        auto graphicsPipeline = std::make_shared<Vulkan::GraphicsPipeline>(config);
        auto material = std::make_shared<Comphi::Material>(graphicsPipeline);
        pool->Add((IGraphicsPipeline*)material.get());
        return material;
    }

    ShaderObjectPtr ComphiAPI::Rendering::Shader(ShaderType shaderType, IFileRef& file, IObjectPool* pool)
    {
        //Vulkan
        auto shaderProgram = std::make_shared<Comphi::Vulkan::ShaderProgram>(shaderType, file);
        auto shader = std::make_shared<Comphi::IShaderProgram>(shaderProgram);
        pool->Add(shader.get());
        return shader;
    }

    MaterialInstancePtr ComphiAPI::Rendering::MaterialInstance(MaterialPtr& parent, IObjectPool* pool)
    {
        //Vulkan
        auto materialInst = std::make_shared<Comphi::MaterialInstance>(parent);
        pool->Add(materialInst.get());
        return materialInst;
    }

    TexturePtr ComphiAPI::Rendering::Texture(IFileRef& fileref, IObjectPool* pool)
    {
        auto imgView = std::make_shared<Vulkan::ImageView>();
        imgView->initTextureImageView(fileref);
        
        auto texture = std::make_shared<Comphi::ITexture>(imgView);
        
        pool->Add(texture.get());
        return texture;
    }

    template<typename T>
    ShaderBufferDataPtr ComphiAPI::Rendering::ShaderBufferData(const T& dataArray, const uint count, BufferUsage usage, IObjectPool* pool)
    {
        auto buffer = std::make_shared<Vulkan::UniformBuffer<T>>(dataArray,count,usage);
        pool->Add(buffer.get());
        return buffer;
    }

    VertexBufferDataPtr ComphiAPI::Rendering::VertexBufferData(const VertexArray& dataArray, IObjectPool* pool)
    {
        auto buffer = Rendering::ShaderBufferData<VertexArray>(dataArray, dataArray.size(), BufferUsage::VertexBuffer, pool);
        pool->Add(buffer.get());
        return buffer;
    }

    IndexBufferDataPtr ComphiAPI::Rendering::IndexBufferData(const IndexArray& dataArray, IObjectPool* pool)
    {
        auto buffer = Rendering::ShaderBufferData<IndexArray>(dataArray, dataArray.size(), BufferUsage::IndexBuffer, pool);
        pool->Add(buffer.get());
        return buffer;
    }

    template<typename T>
    UniformBufferDataPtr ComphiAPI::Rendering::UniformBufferData(const T& dataArray, const uint count, IObjectPool* pool)
    {
        auto buffer = Rendering::ShaderBufferData<T>(dataArray, dataArray.size(), BufferUsage::UniformBuffer, pool);
        pool->Add(buffer.get());
        return buffer;
    }

    MeshObjectPtr ComphiAPI::Rendering::MeshObject(IFileRef& modelFile, MeshBuffers& meshBuffers, IObjectPool* pool)
    {
        auto mesh = std::make_shared<Comphi::MeshObject>(modelFile,meshBuffers);
        pool->Add(mesh.get());
        return mesh;
    }

    MeshObjectPtr ComphiAPI::Rendering::MeshObject(MeshData& data, MeshBuffers& meshBuffers, IObjectPool* pool)
    {
        auto mesh = std::make_shared<Comphi::MeshObject>(data, meshBuffers);
        pool->Add(mesh.get());
        return mesh;
    }

    MeshObjectPtr ComphiAPI::Rendering::MeshObject(VertexArray& vertexData, IndexArray& indexData, MeshBuffers& meshBuffers, IObjectPool* pool)
    {
        auto mesh = std::make_shared<Comphi::MeshObject>(vertexData, indexData, meshBuffers);
        pool->Add(mesh.get());
        return mesh;
    }
   
    /*CameraInstance ComphiAPI::create::Camera(CameraProperties cameraProperties, TransformData transformData, IObjectPool* pool)
    {
        //TODO : Switch RenderingAPI
        std::shared_ptr<Comphi::Camera> camera = std::make_shared<Comphi::Camera>(cameraProperties, transformData);
        pool->Add(camera.get());
        return camera;
    }

    GameObjectInstance ComphiAPI::create::GameObject(MeshData meshData, TransformData transformData, IObjectPool* pool)
    {
        //TODO : Switch RenderingAPI
        std::shared_ptr<Comphi::GameObject> gameObject = std::make_shared<Comphi::GameObject>(meshData, transformData);
        pool->Add(gameObject.get());
        return gameObject;
    }

    SceneInstance ComphiAPI::create::Scene()
    {
        //TODO : Switch RenderingAPI
        return std::make_shared<Comphi::Scene>();
    }

    ShaderInstance ComphiAPI::create::ShaderProgram(Comphi::ShaderType shaderType, IFileRef& shaderFile, IObjectPool* pool)
    {
        //TODO : Switch RenderingAPI
        std::shared_ptr<Vulkan::ShaderProgram> shader = std::make_shared<Vulkan::ShaderProgram>(shaderType, shaderFile);
        pool->Add(shader.get());
        return shader;
    }

    TextureInstance ComphiAPI::create::Texture(IFileRef& fileref, IObjectPool* pool)
    {
        //TODO : Switch RenderingAPI
        std::shared_ptr<Vulkan::ImageView> texture = std::make_shared<Vulkan::ImageView>();
        texture->initTextureImageView(fileref);
        pool->Add(texture.get());
        return texture;
    }

    MaterialInstance ComphiAPI::create::Material(MaterialResources materialProperties, IObjectPool* pool)
    {
        //TODO : Switch RenderingAPI
        std::shared_ptr<Vulkan::Material> material = std::make_shared<Vulkan::Material>(materialProperties);
        pool->Add(material.get());
        return material;
    }

    MeshInstance ComphiAPI::create::Mesh(IFileRef& objFile, MaterialInstance& material, IObjectPool* pool)
    {
        //TODO : Switch RenderingAPI
        std::shared_ptr<Vulkan::MeshObject> mesh = std::make_shared<Vulkan::MeshObject>(objFile, *static_cast<Vulkan::Material*>(material.get()));
        pool->Add(mesh.get());
        return mesh;
    }

    MeshInstance ComphiAPI::create::Mesh(VertexArray& vertices, IndexArray& indices, MaterialInstance& material, IObjectPool* pool)
    {
        //TODO : Switch RenderingAPI
        std::shared_ptr<Vulkan::MeshObject> mesh = std::make_shared<Vulkan::MeshObject>(vertices, indices, *static_cast<Vulkan::Material*>(material.get()));
        pool->Add(mesh.get());
        return mesh;
    }*/

    void ComphiAPI::cleanUpInstances()
    {
        COMPHILOG_CORE_TRACE("Cleaning ComphiAPI Instances...");
        objectPool.cleanUp();
        /*cameraPool.cleanUp();
        gameObjectPool .cleanUp();
        shaderPool     .cleanUp();
        texturePool    .cleanUp();
        materialPool   .cleanUp();
        meshPool       .cleanUp();*/
        COMPHILOG_CORE_TRACE("Finished cleaning ComphiAPI Instances !");
    }





}
