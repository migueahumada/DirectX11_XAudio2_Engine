#include "Dx11DemoBase.h"

Dx11DemoBase::Dx11DemoBase() :	m_driverType (D3D_DRIVER_TYPE_NULL),
								m_featureLevel(D3D_FEATURE_LEVEL_11_0),
								m_pDevice(nullptr),
								m_pDeviceContext(nullptr),
								m_pSwapChain(nullptr),
								m_pBackBufferRTV(nullptr)
								
{

}

Dx11DemoBase::~Dx11DemoBase() {
	Shutdown();
}

bool Dx11DemoBase::Intialize(HINSTANCE hInstance, HWND hwnd)
{

	m_hInstance = hInstance;
	m_hwnd = hwnd;

	RECT dimensions;
	GetClientRect(hwnd, &dimensions);

	unsigned int width = dimensions.right - dimensions.left;
	unsigned int height = dimensions.bottom - dimensions.top;

	D3D_DRIVER_TYPE driverTypes[] = {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_SOFTWARE
	};

	unsigned int totalDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};

	unsigned int totalFeatureLevels = ARRAYSIZE(featureLevels);
	
	DXGI_SWAP_CHAIN_DESC scDesc;
	memset(&scDesc,0,sizeof(DXGI_SWAP_CHAIN_DESC));

	scDesc.BufferCount = 1;
	scDesc.BufferDesc.Width = width;
	scDesc.BufferDesc.Height = height;
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scDesc.BufferDesc.RefreshRate.Numerator = 60;
	scDesc.BufferDesc.RefreshRate.Denominator = 1;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.OutputWindow = hwnd;
	scDesc.Windowed = true;
	scDesc.SampleDesc.Count = 1;
	scDesc.SampleDesc.Quality = 0;

	unsigned int creationFlags = 0;

#ifdef DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // DEBUG

	HRESULT result;
	unsigned int driver = 0;

	for (driver = 0; driver < totalDriverTypes; ++driver)
	{
		result = D3D11CreateDeviceAndSwapChain(
			0, driverTypes[0], 0,
			creationFlags, featureLevels, totalFeatureLevels,
			D3D11_SDK_VERSION, &scDesc, &m_pSwapChain,
			&m_pDevice, &m_featureLevel,&m_pDeviceContext
		);
		if (SUCCEEDED(result))
		{
			m_driverType = driverTypes[driver]; 
			break;
		}
	}
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Failed to create device", L"Error", MB_OK);
		return false;
	}

	ID3D11Texture2D* backBufferTexture = nullptr;

	result = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
				reinterpret_cast<LPVOID*>(& backBufferTexture));
	
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Failed to get the swap chain back buffer!", L"Error", MB_OK);
		return false;
	}

	result = m_pDevice->CreateRenderTargetView(backBufferTexture, 0,
		&m_pBackBufferRTV);

	SAFE_RELEASE(backBufferTexture);

	m_pDeviceContext->OMSetRenderTargets(1, &m_pBackBufferRTV, 0);

	D3D11_VIEWPORT viewport;
	viewport.Width = static_cast<float>(width);
	viewport.Height= static_cast<float>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	m_pDeviceContext->RSSetViewports(1, &viewport);

	//-----------INIT AUDIO------------------------
	result = XAudio2Create(&m_pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Failed to initialize XAudio2!", L"Error", MB_OK);
			return false;
	}
	result = m_pXAudio2->StartEngine();

	
	result = m_pXAudio2->CreateMasteringVoice(&m_pXAudio2MasteringVoice);
	if (FAILED(result))
	{
		MessageBox(m_hwnd, L"Failed to create XAudio2 Mastering Voice!", L"Error", MB_OK);
		return false;
	}

	const WCHAR* path = L"D:\\Coding\\C++\\Direct3DPractice\\rsc\\AudioPrueba.wav";
	const char* path2 = "./rsc/AudioPrueba.wav";
	m_audioFile.load(path2);

	result = m_pXAudio2->CreateSourceVoice(&m_pXAudio2SourceVoice, reinterpret_cast<WAVEFORMATEX*>(&m_audioFile.m_waveFile));

	if (FAILED(result))
	{
		MessageBox(m_hwnd, L"Failed create source Voice!", L"Error", MB_OK);
		return false;
	}

	result = m_pXAudio2SourceVoice->SubmitSourceBuffer(&m_audioFile.m_buffer);
	if (FAILED(result))
	{
		MessageBox(m_hwnd, L"Failed create submit buffer!", L"Error", MB_OK);
		return false;
	}
	result = m_pXAudio2SourceVoice->Start(0);
	if (FAILED(result))
	{
		MessageBox(m_hwnd, L"Failed to play!", L"Error", MB_OK);
		return false;
	}

	//m_audioPrueba.decode2("D:/Coding/C++/Direct3DPractice/AudioPrueba.wav");

	return LoadContent();


}

void Dx11DemoBase::Shutdown()
{
	UnloadContent();

	SAFE_RELEASE(m_pBackBufferRTV);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pDevice);	
	
	//End Audio Stream
	SAFE_RELEASE(m_pXAudio2);
}

bool Dx11DemoBase::LoadContent()
{
	return true;
}

void Dx11DemoBase::UnloadContent()
{

}



