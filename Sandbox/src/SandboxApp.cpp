#include <Comphi.h>
#include <imgui.h>

using namespace Comphi;

class GameSceneLayer : public Comphi::Layer
{
public:
	GameSceneLayer() : Layer("GameSceneLayer") { };

	//TODO: Comphi namepsace objects should use platform & renderer independent interfaces (API)

	MeshInstance meshObj1;

	Comphi::Windows::FileRef textureFile;
	TextureInstance texture1;
	MaterialInstance Albedo1;

	Comphi::Windows::FileRef vert;
	ShaderProgramInstance vertShader;

	ShaderProgramInstance fragShader;
	Comphi::Windows::FileRef frag;

	GameObjectInstance gameObj1;
	Comphi::Windows::FileRef modelMesh;

	CameraInstance camObj1;

	SceneInstance scene1;

	void OnStart() override {
		// ------------------------------------------------------------
		//					GRAPHICS PIPELINE 
		//TODO : Shader/Texture/Material initialization stage -> Move outside of Graphics context

		MaterialProperties materialProperties;
		textureFile = Windows::FileRef("textures/viking_room.png");
		texture1 = MakeTextureInstance(textureFile);

		ShaderTextures textures = { texture1.get()};
		materialProperties.shaderTextures = textures;

		vert = Windows::FileRef("shaders\\vert.spv");
		frag = Windows::FileRef("shaders\\frag.spv");
		vertShader = MakeShaderProgramInstance(ShaderType::VertexShader, vert);
		fragShader = MakeShaderProgramInstance(ShaderType::FragmentShader, frag);

		std::vector<IShaderProgram*> shaders = { &*vertShader , &*fragShader };
		materialProperties.shaders = shaders;

		//shared Material Instance
		Albedo1 = MakeMaterialInstance(materialProperties);


		// ------------------------------------------------------------
		//					TEST OBJECTS PIPELINE 
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

		modelMesh = Windows::FileRef("models/viking_room.obj");
		meshObj1 = MakeMeshInstance(modelMesh, *Albedo1.get());
		gameObj1 = MakeGameObjectInstance(meshObj1);
		gameObj1->action.updateCallback = [this](Time frameTime,void*) {
			gameObj1->transform.position = glm::vec3(0, 0, glm::sin(frameTime.deltaTime()));
			gameObj1->transform.setEulerAngles(glm::vec3(0.0f, 0.0f, 45.0f) * frameTime.deltaTime());
		};

		camObj1 = MakeCameraInstance();
		camObj1->transform.position = glm::vec3(0.0f, 4.0f, 0.0f);

		camObj1->action.updateCallback = [this](Time frameTime,void*) {
			camObj1->transform.lookAt(gameObj1->transform.position);
		};

	}

	void OnEnd() override {};

	void OnUpdate() override {};

	void OnUIRender() override {};

	void OnEvent(Comphi::Event& e) override {};

private:
};

class Sandbox : public Comphi::Application
{
public:
	Sandbox() {
		GameSceneLayer* Renderlayer = new GameSceneLayer();
		PushLayer(*Renderlayer);
		PushScene(Renderlayer->scene1);
	}
	~Sandbox(){}
private:
};

Comphi::Application* Comphi::CreateApplication() {
	return new Sandbox();
}