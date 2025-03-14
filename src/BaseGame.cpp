#include "BaseGame.h"

BaseGame::BaseGame()
{
}

BaseGame::~BaseGame()
{
	Shutdown();
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

bool BaseGame::Init(HINSTANCE hInstance, HWND hwnd) 
{

	m_hwnd = hwnd;

	m_pGraphicsAPI = make_unique<GraphicsAPI>(hwnd);
	if (!m_pGraphicsAPI)
	{
		MessageBox(hwnd,L"GraphicsAPI couldn't be created",L"Error",MB_OK);
		return false;
	}

	m_pAudioAPI = make_unique<AudioAPI>(hwnd);
	if (!m_pAudioAPI)
	{
		MessageBox(hwnd, L"AudioAPI couldn't be created", L"Error", MB_OK);
		return false;
	}

	m_pVertexShader = m_pGraphicsAPI->CreateVertexShader(L"./rsc/BasicShader.hlsl","vertex_main" );
	if (!m_pVertexShader)
	{
		MessageBox(hwnd, L"VertexShader couldn't be created", L"Error", MB_OK);
		return false;
	}

	m_pPixelShader = m_pGraphicsAPI->CreatePixelShader(L"./rsc/BasicShader.hlsl", "pixel_main");
	if (!m_pPixelShader)
	{
		MessageBox(hwnd, L"PixelShader couldn't be created", L"Error", MB_OK);
		return false;
	}

	
	Vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"COLOR"	, 0, DXGI_FORMAT_R32G32B32_FLOAT,  0, 12,D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	m_pInputLayout = m_pGraphicsAPI->CreateInputLayout(inputElementDesc,m_pVertexShader);
	if (!m_pInputLayout)
	{
		return false;
	}

	MODEL_VERTEX vertices[] =
	{
		{	0.0f,  0.5f, 0.0f , 1, 0, 0},
		{	0.5f, -0.5f, 0.0f , 0, 1, 0},
		{  -0.5f, -0.5f, 0.0f , 0, 0, 1},
	};

	Vector<char> data;
	data.resize(sizeof(vertices));
	memcpy(data.data(),vertices,sizeof(vertices));

	m_pVertexBuffer = m_pGraphicsAPI->CreateVertexBuffer(data);
	if (!m_pVertexBuffer)
	{
		return false;
	}

	pSound = m_pAudioAPI->CreateSoundEffect("Mark","./rsc/AudioPrueba.wav");
	pSound->m_pSourceVoice->SetVolume(0.04f);
	m_pAudioAPI->Play(pSound);     

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
	ImGui_ImplWin32_Init(m_hwnd);
	ImGui_ImplDX11_Init(m_pGraphicsAPI.get()->m_pDevice, m_pGraphicsAPI.get()->m_pDeviceContext);
	

	return true;
}

void BaseGame::Shutdown()
{

	
}

bool BaseGame::LoadContent()
{

	return true;
}

void BaseGame::UnloadContent()
{

}

void BaseGame::Update(float dt)
{
	// (Your code process and dispatch Win32 messages)
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();



	D3D11_VIEWPORT vp;
	vp.Width = 480;
	vp.Height = 720;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	//Rellenar el Input Assembler
	m_pGraphicsAPI->m_pDeviceContext->OMSetRenderTargets(1,
		&m_pGraphicsAPI->m_pBackBufferRTV,
		m_pGraphicsAPI->m_pBackBufferDSV);

	float clearColor[4] = { 0.5f,0.2f,0.5f,0.5f };

	m_pGraphicsAPI->m_pDeviceContext->ClearRenderTargetView(m_pGraphicsAPI->m_pBackBufferRTV, clearColor);


	m_pGraphicsAPI->m_pDeviceContext->ClearDepthStencilView(m_pGraphicsAPI->m_pBackBufferDSV,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);
	m_pGraphicsAPI->m_pDeviceContext->VSSetShader(m_pVertexShader->m_pVertexShader, nullptr, 0);
	m_pGraphicsAPI->m_pDeviceContext->PSSetShader(m_pPixelShader->m_pPixelShader, nullptr, 0);

	m_pGraphicsAPI->m_pDeviceContext->IASetInputLayout(m_pInputLayout);
	m_pGraphicsAPI->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	UINT stride = sizeof(MODEL_VERTEX);
	UINT offset = 0;
	m_pGraphicsAPI->m_pDeviceContext->IASetVertexBuffers(0,
		1,
		&m_pVertexBuffer->m_pBuffer,
		&stride,
		&offset);

	m_pGraphicsAPI->m_pDeviceContext->Draw(3, 0);
	//g_pGraphicsAPI->m_pDeviceContext->DrawIndexed();


	ImGui::Begin("Hola");
	ImGui::Text("asdfhasjdhfkljashdlfkjhasdjklf");
	ImGui::End();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
	
	//Presenting
	m_pGraphicsAPI->m_pSwapChain->Present(0, 0);

}

void BaseGame::Render()
{
	// Rendering
// (Your code clears your framebuffer, renders your other stuff etc.)
	
	// (Your code calls swapchain's Present() function)
}
