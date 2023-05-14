#include "cphipch.h"
#include "ImGuiLayer.h"

#include "Comphi/Core/Application.h"
#include "Comphi/API/ComphiAPI.h"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>
#include <GLFW/glfw3.h>

namespace Comphi {

	ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer")
	{
		
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		
		
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		
		
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;

		// Setup Dear ImGui style
		ImGui::StyleColorsLight();
		//ImGui::StyleColorsClassic();
		//ImGui::StyleColorsDark();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Setup Platform/Renderer bindings
		//auto window = std::static_pointer_cast<GLFWwindow>(Application::Get().GetWindow().GetNativeWindow());
		auto window = static_cast<GLFWwindow*>((Application::Get().GetWindowHandler().GetNativeWindow()));

		switch (ComphiAPI::getActiveAPI()) {
		case ComphiAPI::Vulkan:
			ImGui_ImplGlfw_InitForVulkan(window , true);
			//ImGui_ImplVulkan_Init(NULL,NULL); //!fix
			break;
		default:
			COMPHILOG_CORE_ERROR("No rendering API Selected.");
			break;
		}

		// Load Fonts
		// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
		// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
		// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
		// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
		// - Read 'docs/FONTS.md' for more instructions and details.
		// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
		ImFont* font = io.Fonts->AddFontDefault();
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
		//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
		IM_ASSERT(font != NULL);

		// Our state
		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		

	}

	void ImGuiLayer::OnDetach()
	{
		
		switch (ComphiAPI::getActiveAPI()) {
		case ComphiAPI::Vulkan:
			ImGui_ImplVulkan_Shutdown();
			break;
		default:
			COMPHILOG_CORE_ERROR("No rendering API Selected.");
			break;
		}
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		
		
	}

	void ImGuiLayer::Begin()
	{
		
		switch (ComphiAPI::getActiveAPI()) {
		case ComphiAPI::Vulkan:
			ImGui_ImplVulkan_NewFrame();
			break;
		default:
			COMPHILOG_CORE_ERROR("No rendering API Selected.");
			break;
		}

		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
	}
		
	
	void ImGuiLayer::End()
	{
		
		ImGuiIO io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindowHandler().GetWidth(), (float)app.GetWindowHandler().GetHeight());

		//Rendering 
		ImGui::Render();

		switch (ComphiAPI::getActiveAPI()) {
		case ComphiAPI::Vulkan:
			ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(),NULL); //TODO: Fix imGUI for vulkan
			break;
		default:
			COMPHILOG_CORE_ERROR("No rendering API Selected.");
			break;

		}

		//Docking Branch:
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
		
	}


	void ImGuiLayer::OnUIRender()
	{
		//ImGui::ShowDemoWindow(&m_Enabled);
	}

}

