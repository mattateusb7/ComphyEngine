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
	simpleMaterial->configuration.rasterizerSettings.cullMode = CullingMode::NoCulling;
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
	
	gameObjA = ComphiAPI::CreateObject::Entity();
	auto& transformComponent = gameObjA->AddComponent(ComphiAPI::CreateComponent::Transform());
	gameObjA->AddComponent(ComphiAPI::CreateComponent::Renderer(meshObjA, AlbedoA));

	CameraObj = ComphiAPI::CreateObject::Entity();
	auto& cameraTransform = CameraObj->AddComponent(ComphiAPI::CreateComponent::Transform());
	auto& cameraComponent = CameraObj->AddComponent(ComphiAPI::CreateComponent::Camera());
	
	cameraTransform->position = glm::vec3(0.0, 0.0f, 0.0f);
	cameraTransform->lookAt(glm::vec3(1.0f, 0.0f, 0.0f));
	cameraComponent->properties.FOV = 70;

	scene = ComphiAPI::CreateObject::Scene();
	scene->addEntity(CameraObj);
	scene->addEntity(gameObjA);

	//gameObjA->GetComponent<Renderer>()->;
	//gameObj1->action.updateCallback = [this](Time frameTime,void*) { //TODO: fix Lambda not defined when out of scope
	//	gameObj1->transform.position = glm::vec3(0, 0, glm::sin(frameTime.deltaTime()));
	//	gameObj1->transform.setEulerAngles(glm::vec3(0.0f, 0.0f, 45.0f) * frameTime.deltaTime());
	//};

	//GameObject2
	/*modelMesh1 = Windows::FileRef("models/BLEPOSPACE.obj");
	meshObj1 = ComphiAPI::CreateObject::Mesh(modelMesh1, Albedo1); //TODO: fix materials / descriptor sets not share-able ...
	gameObj1 = ComphiAPI::CreateObject::GameObject({ meshObj1 }, { gameObj.get() });

	//Camera
	Transform scaleup  = Transform();
	scaleup.scale = glm::vec3(1.0f, 1.0f, 1.0f);
	emptyObj = ComphiAPI::CreateObject::GameObject({ ComphiAPI::CreateObject::Mesh(cubeVx, CubeIx, Albedo2) }, { nullptr, scaleup });
	camObj = ComphiAPI::CreateObject::Camera({}, { emptyObj.get() });

	*/

}

void GameSceneLayer::OnStart()
{

}

void GameSceneLayer::OnUpdate()
{
	time.Stop(); //TODO: send as parameter ?

	//gameObjA->GetComponent<Transform>()->position = glm::vec3(-1.0f,0.0f,0.0f);
	/*
	

	CameraObj->GetComponent<Transform>()->position = glm::vec3(0.0, 3.0f, 1.0f);
	CameraObj->GetComponent<Transform>()->lookAt(glm::vec3(0.0, 0.0f, 0.0f));
	CameraObj->GetComponent<Transform>()->setEulerAngles(glm::vec3(0.0, 0.0f, 0.0f));
	//CameraObj->GetComponent<Transform>()->eulerRotation(glm::vec3(0.0f, 45.0f * time.deltaTime(), 0));
	CameraObj->GetComponent<Camera>()->properties.FOV = 70;

	//camObj->transform.position = glm::vec3(0.0, -3.0f, 1.0f);
	////camObj->transform.parent = &emptyObj->transform;
	//camObj->transform.parent = &emptyObj->transform;
	////camObj->transform.parent = nullptr;
	//emptyObj->transform.parent = &gameObj1->transform;
	*/
	gameObjA->GetComponent<Transform>()->eulerRotation(glm::vec3(0.0f, 0.0f, time.deltaTime() * -45.0f));
	/*

	//gameObj->transform.eulerRotation(glm::vec3(0.0f, 0.0f, time.deltaTime() * 0.0f));
	//gameObj->transform.position = (glm::vec3(0.0f, 0.0f, glm::sin(time.sinceBegining())/2.0f));
	//
	//gameObj1->transform.position = (glm::vec3(glm::cos(time.sinceBegining())/2.0f, 0, 1.0f));
	//
	//camObj->FOV = 70;
	//camObj->transform.lookAt(glm::vec3(0.0f,0.0f,0.0f));
	//camObj->transform.setEulerAngles(glm::vec3(0,0,0));
	//camObj->transform.eulerRotation(glm::vec3(0.0f, 45.0f * time.deltaTime(), 0));
	
	*/

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