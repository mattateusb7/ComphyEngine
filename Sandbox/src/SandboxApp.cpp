#include <Comphi.h>
#include <imgui.h>

using namespace Comphi;

class GameSceneLayer : public Comphi::Layer
{
public:

	//TODO: Comphi namepsace objects should use platform & renderer independent interfaces (API)

	Windows::FileRef textureFile;
	Windows::FileRef textureFile2;
	TextureInstance texture;
	TextureInstance texture2;

	Windows::FileRef vert;
	ShaderInstance vertShader;

	Windows::FileRef frag;
	ShaderInstance fragShader;

	MaterialInstance Albedo;
	MaterialInstance Albedo1;
	MaterialInstance Albedo2;

	Windows::FileRef modelMesh;
	MeshInstance meshObj;	

	Windows::FileRef modelMesh1;
	MeshInstance meshObj1;

	GameObjectInstance gameObj;
	GameObjectInstance gameObj1;
	GameObjectInstance emptyObj;

	CameraInstance camObj;

	SceneInstance scene;

	GameSceneLayer() : Layer("GameSceneLayer") { 

		//Texture
		textureFile = Windows::FileRef("textures/viking_room.png");
		texture = GraphicsAPI::create::Texture(textureFile);

		textureFile2 = Windows::FileRef("textures/lain.jpg");
		texture2 = GraphicsAPI::create::Texture(textureFile2);

		//Shaders
		MaterialProperties materialProperties;
		materialProperties.shaderTextures = { texture.get() };

		vert = Windows::FileRef("shaders/vert.spv");
		frag = Windows::FileRef("shaders/frag.spv");
		vertShader = GraphicsAPI::create::ShaderProgram(ShaderType::VertexShader, vert);
		fragShader = GraphicsAPI::create::ShaderProgram(ShaderType::FragmentShader, frag);

		ShaderPrograms AlbedoShader = { vertShader.get() , fragShader.get() };
		materialProperties.shaderPrograms = AlbedoShader;

		//Materials
		Albedo = GraphicsAPI::create::Material(materialProperties);

		materialProperties.shaderTextures = { texture2.get() };
		Albedo1 = GraphicsAPI::create::Material(materialProperties);
		Albedo2 = GraphicsAPI::create::Material(materialProperties);

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

		//GameObject1
		modelMesh = Windows::FileRef("models/viking_room.obj");
		meshObj = GraphicsAPI::create::Mesh(modelMesh, Albedo);
		gameObj = GraphicsAPI::create::GameObject({ meshObj });
		//gameObj1->action.updateCallback = [this](Time frameTime,void*) { //TODO: fix Lambda not defined when out of scope
		//	gameObj1->transform.position = glm::vec3(0, 0, glm::sin(frameTime.deltaTime()));
		//	gameObj1->transform.setEulerAngles(glm::vec3(0.0f, 0.0f, 45.0f) * frameTime.deltaTime());
		//};

		//GameObject2
		modelMesh1 = Windows::FileRef("models/BLEPOSPACE.obj");
		meshObj1 = GraphicsAPI::create::Mesh(modelMesh1, Albedo1); //TODO: fix materials / descriptor sets not share-able ...
		gameObj1 = GraphicsAPI::create::GameObject({ meshObj1 }, { gameObj.get() });

		//Camera
		Transform scaleup  = Transform();
		scaleup.scale = glm::vec3(1.0f, 1.0f, 1.0f);
		emptyObj = GraphicsAPI::create::GameObject({ GraphicsAPI::create::Mesh(cubeVx, CubeIx, Albedo2) }, { nullptr, scaleup });
		camObj = GraphicsAPI::create::Camera({}, { emptyObj.get() });

		scene = GraphicsAPI::create::Scene();
		scene->sceneObjects.push_back(gameObj);
		scene->sceneObjects.push_back(gameObj1);
		scene->sceneObjects.push_back(emptyObj);
		scene->sceneCamera = (camObj);
	
	}; 
	
	void OnStart() override {
		gameObj1->transform.position = glm::vec3(0, 0, 1.0f);
	}

	Time time;
	void OnUpdate() override {
		//Works with HotReloadinng <3 ! (only allows manipulation of already sent data)
		//recompiling Sandbox.exe refreshes the code without the need to recompile the engine (simply re-links the static engine lib ).
		//TODO: detect when a hotreload is done and call "UpdateOnce after hotreload" function. (to send new values) 

		time.Stop(); //TODO: send as parameter

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

		time.Start();
	};

	void OnUIRender() override {};

	void OnEvent(Comphi::Event& e) override 
	{
		//Called once per event 
		//TODO: we Need to Abstract this into general hooks that GET events instead of sorting it out here  
	};

	void OnEnd() override {};

private:
};

class Sandbox : public Comphi::Application
{
public:
	Sandbox() {
		GameSceneLayer* Renderlayer = new GameSceneLayer();
		PushLayer(*Renderlayer);
		PushScene(Renderlayer->scene);
	}
private:
};

Comphi::Application* Comphi::CreateApplication() {
	return new Sandbox();
}