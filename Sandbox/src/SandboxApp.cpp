#include "SandboxApp.h"

GameSceneLayer::GameSceneLayer() : Layer("GameSceneLayer") {


	//Mesh
	const VertexArray vertices = {
		{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
		{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
		{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

		{{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
		{{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
		{{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
	};

	const IndexArray indices = {
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4
	};

	VertexArray cubeVx = {
		{{ 0.5f, 0.5f, 0.5f} , {1.0f, 1.0f, 1.0f} , {1.0f, 0.0f}},
		{{-0.5f, 0.5f, 0.5f} , {1.0f, 1.0f, 0.0f} , {0.0f, 0.0f}},
		{{-0.5f,-0.5f, 0.5f} , {1.0f, 0.0f, 0.0f} , {0.0f, 1.0f}},
		{{ 0.5f,-0.5f, 0.5f} , {1.0f, 0.0f, 1.0f} , {1.0f, 1.0f}},
		{{ 0.5f,-0.5f,-0.5f} , {0.0f, 0.0f, 1.0f} , {1.0f, 0.0f}},
		{{ 0.5f, 0.5f,-0.5f} , {0.0f, 1.0f, 1.0f} , {0.0f, 0.0f}},
		{{-0.5f, 0.5f,-0.5f} , {0.0f, 1.0f, 0.0f} , {0.0f, 1.0f}},
		{{-0.5f,-0.5f,-0.5f} , {0.0f, 0.0f, 0.0f} , {1.0f, 1.0f}}
	};

	IndexArray cubeIx = {
		0, 1, 2,   2, 3, 0,   // v0-v1-v2, v2-v3-v0 (front)
		0, 3, 4,   4, 5, 0,   // v0-v3-v4, v4-v5-v0 (right)
		0, 5, 6,   6, 1, 0,   // v0-v5-v6, v6-v1-v0 (top)
		1, 6, 7,   7, 2, 1,   // v1-v6-v7, v7-v2-v1 (left)
		7, 4, 3,   3, 2, 7,   // v7-v4-v3, v3-v2-v7 (bottom)
		4, 7, 6,   6, 5, 4    // v4-v7-v6, v6-v5-v4 (back)
	};

	vert = Windows::FileRef("shaders/vert.spv");
	frag = Windows::FileRef("shaders/frag.spv");
	vertShader = ComphiAPI::CreateObject::Shader(ShaderType::VertexShader, vert);
	fragShader = ComphiAPI::CreateObject::Shader(ShaderType::FragmentShader, frag);
	
	//Material / Graphics Pipeline
	simpleMaterial = ComphiAPI::CreateObject::Material();
	simpleMaterial->addDefaultVertexBindingDescription();
	simpleMaterial->createShaderResourceLayoutSetDescriptorSetBinding(PerMaterialInstance, 0, 1, UniformBufferData); //Camera ViewProjectionMatrix (& Lights)
	simpleMaterial->createShaderResourceLayoutSetDescriptorSetBinding(PerMaterialInstance, 1, 1, ImageBufferSampler, ShaderStageFlag::FragmentStage); //Textures
	simpleMaterial->createShaderResourceLayoutSetDescriptorSetBinding(PerMaterialInstance, 2, 1, UniformBufferData); //Mesh & ModelMatrix 
	simpleMaterial->addShader(vertShader);
	simpleMaterial->addShader(fragShader);
	simpleMaterial->configuration.rasterizerSettings.cullMode = CullingMode::BackCulling;
	simpleMaterial->initialize();
	
	//Mesh 1
	modelMeshA = Windows::FileRef("models/viking_room.obj");
	meshObjA = ComphiAPI::CreateObject::MeshObject(modelMeshA);

	cubeVX = ComphiAPI::CreateObject::MeshObject(cubeVx, cubeIx);

	//Texture
	textureFile = Windows::FileRef("textures/viking_room.png");
	texture = ComphiAPI::CreateObject::Texture(textureFile);
	
	textureFile2 = Windows::FileRef("textures/lain.jpg");
	texture2 = ComphiAPI::CreateObject::Texture(textureFile2);
	
	//MaterialInstances
	AlbedoA = ComphiAPI::CreateObject::MaterialInstance(simpleMaterial);
	AlbedoA->bindTexture(texture, PerMaterialInstance, 1);

	AlbedoB = ComphiAPI::CreateObject::MaterialInstance(simpleMaterial);
	AlbedoB->bindTexture(texture2, PerMaterialInstance, 1);
	
	gameObjA = ComphiAPI::CreateObject::Entity();
	auto& transformComponent = gameObjA->AddComponent(ComphiAPI::CreateComponent::Transform());
	gameObjA->AddComponent(ComphiAPI::CreateComponent::Renderer(meshObjA, AlbedoA));

	gameObjB = ComphiAPI::CreateObject::Entity();
	auto& transformComponentB = gameObjB->AddComponent(ComphiAPI::CreateComponent::Transform());
	gameObjB->AddComponent(ComphiAPI::CreateComponent::Renderer(cubeVX, AlbedoB));

	gameObjC = ComphiAPI::CreateObject::Entity();
	auto& transformComponentC = gameObjC->AddComponent(ComphiAPI::CreateComponent::Transform());
	gameObjC->AddComponent(ComphiAPI::CreateComponent::Renderer(cubeVX, AlbedoA));
	//diff mesh, same materal inst works (1 draw call per object)
	// 
	//gameObjC->AddComponent(ComphiAPI::CreateComponent::Renderer(cubeVX, AlbedoB));
	//same mesh, diff matInst fails in instanced rendering <<< WIP Mesh Instancing

	CameraObj = ComphiAPI::CreateObject::Entity();
	auto& cameraTransform = CameraObj->AddComponent(ComphiAPI::CreateComponent::Transform());
	auto& cameraComponent = CameraObj->AddComponent(ComphiAPI::CreateComponent::Camera());
	
	scene = ComphiAPI::CreateObject::Scene();
	scene->addEntity(CameraObj);
	scene->addEntity(gameObjA);
	scene->addEntity(gameObjB);
 	//scene->addEntity(gameObjC);

}

void GameSceneLayer::OnStart()
{
	gameObjA->GetComponent<Transform>()->setEulerAngles(glm::vec3(-90, 45, 0));
	gameObjA->GetComponent<Transform>()->scale = glm::vec3(3, 3, 3);

	gameObjB->GetComponent<Transform>()->scale = glm::vec3(0.5f, 0.5f, 0.5f);

	//CameraObj->GetComponent<Transform>()->parent = gameObjA->GetComponent<Transform>();
	//CameraObj->GetComponent<Transform>()->lookAt(glm::vec3(0.0f, 0.0f, 0.0f));
	CameraObj->GetComponent<Transform>()->setEulerAngles(glm::vec3(0.0, 0.0f, 0.0f));
	CameraObj->GetComponent<Camera>()->properties.FOV = 70;
	//CameraObj->GetComponent<Transform>()->parent = gameObjB->GetComponent<Transform>();

}

void GameSceneLayer::OnUpdate()
{
	time.Stop(); //TODO: send as parameter ?

	CameraObj->GetComponent<Transform>()->position = glm::vec3(0.5f, 1.0f, -2 + glm::sin(time.sinceBegining()) / 2.0f);

	gameObjB->GetComponent<Transform>()->position = glm::vec3(glm::sin(time.sinceBegining()), 1 + glm::sin(time.sinceBegining()) / 2.0f, glm::cos(time.sinceBegining()));
	gameObjB->GetComponent<Transform>()->eulerRotation(glm::vec3(glm::sin(time.sinceBegining())/3.0f, 1 + glm::sin(time.sinceBegining()) / 2.0f, glm::cos(time.sinceBegining())/2.0f ));
	
	gameObjA->GetComponent<Transform>()->eulerRotation(glm::vec3(0, 0, 10.0f * time.deltaTime()));

	time.Start(); //TODO : Start automatically after scripts ?
}

void GameSceneLayer::OnUIRender()
{

}

void GameSceneLayer::OnEvent(Comphi::Event& e)
{
	//Called once per event 
	//TODO: we Need to Abstract this into general hooks that GET events instead of sorting it out here  
};

class Sandbox : public Comphi::Application
{
public:
	Sandbox() {
		this->windowProperties.Width = 640;
		this->windowProperties.Width = 528;

		GameSceneLayer* Renderlayer = new GameSceneLayer();
		PushLayer(*Renderlayer);
		PushScene(Renderlayer->scene);
	}
private:
};

Comphi::Application* Comphi::CreateApplication() {
	return new Sandbox();
}