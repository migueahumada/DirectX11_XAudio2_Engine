#include "ImGuiAPI.h"

ImGuiAPI::ImGuiAPI()
{
}

ImGuiAPI::~ImGuiAPI()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

bool ImGuiAPI::Init(void* hwnd, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	m_hwnd = static_cast<HWND>(hwnd);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigDockingAlwaysTabBar = true;
	io.ConfigDockingTransparentPayload = true;
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	if (!ImGui_ImplWin32_Init(m_hwnd))
	{
		MessageBox(m_hwnd,L"Couldn't initialize imgui",L"Error",MB_OK);
		return false;
	}
	
	if (!ImGui_ImplDX11_Init(pDevice, pDeviceContext))
	{
		MessageBox(m_hwnd, L"Couldn't initialize imgui", L"Error", MB_OK);
		return false;
	}

	return true;
}

void ImGuiAPI::Update()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiAPI::Render()
{
	ImGui::Begin("Hola");
	ImGui::Text("asdfhasjdhfkljashdlfkjhasdjklf");
	ImGui::End();
	
	static int counter = 0;
	if (ImGui::Button("Play"))
	{
		MessageBoxA(m_hwnd, "PLAYED" , "Cool!", MB_OK);
		counter++;
	}

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
}
