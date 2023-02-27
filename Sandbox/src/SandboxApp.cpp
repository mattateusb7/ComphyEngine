#include <Comphi.h>

using namespace Comphi;

class GameSceneLayer : public Comphi::Layer
{
public:

	IObjectPool poolA;

	//Shaders
	Windows::FileRef vert;
	Windows::FileRef frag;
	ShaderObjectPtr vertShader;
	ShaderObjectPtr fragShader;

	//Model A
	Windows::FileRef modelMeshA;
	MeshData meshDataA;
	MeshBuffers meshBuffersA;
	MeshObjectPtr meshObjA;

	//Cube Model B
	MeshObjectPtr cubeVX;
	MeshBuffers meshBuffersB;

	//Textures
	Windows::FileRef textureFile;
	Windows::FileRef textureFile2;
	TexturePtr texture;
	TexturePtr texture2;

	//Material and instances
	MaterialPtr simpleMaterial;
	MaterialInstancePtr AlbedoA;
	MaterialInstancePtr AlbedoB;

	//GameObjects
	EntityPtr gameObjA;
	EntityPtr CameraObj;
	EntityPtr emptyObj;

	SceneGraphPtr scene;

	GameSceneLayer() : Layer("GameSceneLayer") { 


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

		IndexArray CubeIx = {
			0, 1, 2,   2, 3, 0,   // v0-v1-v2, v2-v3-v0 (front)
			0, 3, 4,   4, 5, 0,   // v0-v3-v4, v4-v5-v0 (right)
			0, 5, 6,   6, 1, 0,   // v0-v5-v6, v6-v1-v0 (top)
			1, 6, 7,   7, 2, 1,   // v1-v6-v7, v7-v2-v1 (left)
			7, 4, 3,   3, 2, 7,   // v7-v4-v3, v3-v2-v7 (bottom)
			4, 7, 6,   6, 5, 4    // v4-v7-v6, v6-v5-v4 (back)
		};

		//meshBuffersA.indexBuffer = ComphiAPI::Rendering::IndexBufferData()

	/*  modelMeshA = Windows::FileRef("models/viking_room.obj");
		meshObjA = ComphiAPI::Rendering::MeshObject(modelMeshA, meshBuffersA);
		cubeVX = ComphiAPI::Rendering::MeshObject(cubeVx,CubeIx, meshBuffersB);
		vert = Windows::FileRef("shaders/vert.spv");
		frag = Windows::FileRef("shaders/frag.spv");
		vertShader = ComphiAPI::Rendering::Shader(ShaderType::VertexShader, vert);
		fragShader = ComphiAPI::Rendering::Shader(ShaderType::FragmentShader, frag);

		//Material / Graphics Pipeline
		simpleMaterial = ComphiAPI::Rendering::Material();
		simpleMaterial->addDefaultVertexBindingDescription();
		simpleMaterial->addShaderResource(0, 1, ShaderResourceDescriptorType::UniformBufferData); //Camera projection (& Lights)
		simpleMaterial->addShaderResource(1, 1, ShaderResourceDescriptorType::ImageBufferSampler); //Material resources : Texture
		simpleMaterial->addShaderResource(2, 1, ShaderResourceDescriptorType::UniformBufferData); //Mesh ModelViewMatrix 
		simpleMaterial->addShader(vertShader);
		simpleMaterial->addShader(fragShader);
		simpleMaterial->initialize();

		//Texture
		textureFile = Windows::FileRef("textures/viking_room.png");
		texture = ComphiAPI::Rendering::Texture(textureFile);

		textureFile2 = Windows::FileRef("textures/lain.jpg");
		texture2 = ComphiAPI::Rendering::Texture(textureFile2);

		//MaterialInstances
		AlbedoA = ComphiAPI::Rendering::MaterialInstance(simpleMaterial);
		AlbedoA->bindTextures(texture2, 0, 1, PerScene);
		AlbedoA->bindTextures(texture,	1, 1, PerMaterialInstance);

		//GameObject1
		gameObjA = ComphiAPI::SceneGraph::Entity();
		gameObjA->AddComponent(ComphiAPI::Components::Transform());
		gameObjA->AddComponent(ComphiAPI::Components::Renderer(meshObjA,AlbedoA));
		
		//auto ModelViewMatrix = Comphi::ComphiAPI::Rendering::UniformBufferData(gameObjA->GetComponent<Transform>()->getModelViewMatrix(), 1);
		//ModelViewMatrix->updateBufferData()
		//gameObjA->GetComponent<Transform>()->getModelMatrix();
		
		//AlbedoA->bindBuffers(, 1, PerMeshObject);

		CameraObj = ComphiAPI::SceneGraph::Entity();
		CameraObj->AddComponent(ComphiAPI::Components::Transform());
		CameraObj->AddComponent(ComphiAPI::Components::Camera());

		//auto camProjMatrixBuff = Comphi::ComphiAPI::Rendering::ShaderBufferData(CameraObj->GetComponent<Camera>()->getProjectionMatrix(), 1);
		//camProjMatrixBuff->updateBufferData(&CameraObj->GetComponent<Camera>()->getProjectionMatrix());
		
		//AlbedoA->bindBuffers(camProjMatrixBuff, 1, PerScene);

		scene = ComphiAPI::SceneGraph::Scene();
		scene->addEntity(gameObjA);
		scene->addEntity(CameraObj);

		//gameObjA->GetComponent<Renderer>()->;
		//gameObj1->action.updateCallback = [this](Time frameTime,void*) { //TODO: fix Lambda not defined when out of scope
		//	gameObj1->transform.position = glm::vec3(0, 0, glm::sin(frameTime.deltaTime()));
		//	gameObj1->transform.setEulerAngles(glm::vec3(0.0f, 0.0f, 45.0f) * frameTime.deltaTime());
		//};

		//GameObject2
		/*modelMesh1 = Windows::FileRef("models/BLEPOSPACE.obj");
		meshObj1 = ComphiAPI::Rendering::Mesh(modelMesh1, Albedo1); //TODO: fix materials / descriptor sets not share-able ...
		gameObj1 = ComphiAPI::Rendering::GameObject({ meshObj1 }, { gameObj.get() });

		//Camera
		Transform scaleup  = Transform();
		scaleup.scale = glm::vec3(1.0f, 1.0f, 1.0f);
		emptyObj = ComphiAPI::Rendering::GameObject({ ComphiAPI::Rendering::Mesh(cubeVx, CubeIx, Albedo2) }, { nullptr, scaleup });
		camObj = ComphiAPI::Rendering::Camera({}, { emptyObj.get() });

		scene = ComphiAPI::Rendering::Scene();
		scene->sceneObjects.push_back(gameObj);
		scene->sceneObjects.push_back(gameObj1);
		scene->sceneObjects.push_back(emptyObj);
		scene->sceneCamera = (camObj);
		*/
	
	}; 
	
	void OnStart() override {
		//gameObj1->transform.position = glm::vec3(0, 0, 1.0f);
	}

	Time time;
	void OnUpdate() override {
		
		//Works with HotReloadinng <3 ! (only allows manipulation of already sent data)
		//recompiling Sandbox.exe refreshes the code without the need to recompile the engine (simply re-links the static engine lib ).
		//TODO: detect when a hotreload is done and call "UpdateOnce after hotreload" function. (to send new values) 

		/*
		time.Stop(); //TODO: send as parameter ?

		camObj->transform.position = glm::vec3(0.0, -3.0f, 1.0f);
		//camObj->transform.parent = &emptyObj->transform;
		camObj->transform.parent = &emptyObj->transform;
		//camObj->transform.parent = nullptr;
		emptyObj->transform.parent = &gameObj1->transform;

		emptyObj->transform.eulerRotation(glm::vec3(0.0f, 0.0f, time.deltaTime() * -45.0f));

		gameObj->transform.eulerRotation(glm::vec3(0.0f, 0.0f, time.deltaTime() * 0.0f));
		gameObj->transform.position = (glm::vec3(0.0f, 0.0f, glm::sin(time.sinceBegining())/2.0f));

		gameObj1->transform.position = (glm::vec3(glm::cos(time.sinceBegining())/2.0f, 0, 1.0f));

		camObj->FOV = 70;
		//camObj->transform.lookAt(glm::vec3(0.0f,0.0f,0.0f));
		//camObj->transform.setEulerAngles(glm::vec3(0,0,0));
		//camObj->transform.eulerRotation(glm::vec3(0.0f, 45.0f * time.deltaTime(), 0));

		time.Start(); //TODO : Start automatically after scripts ?
		*/
	};

	void OnUIRender() override {};

	void OnEvent(Comphi::Event& e) override 
	{
		//Called once per event 
		//TODO: we Need to Abstract this into general hooks that GET events instead of sorting it out here  
	};

	void OnEnd() override {
		ComphiAPI::cleanUpInstances();
	};

private:
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