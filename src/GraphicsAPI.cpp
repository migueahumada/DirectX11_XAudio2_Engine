#include "GraphicsAPI.h"

GraphicsAPI::GraphicsAPI(void* pWindowHandle) : m_pHwnd(pWindowHandle)
{
	HRESULT result;

	HWND hwnd = static_cast<HWND>(pWindowHandle);	//Convierte el puntero vacío a HWND

	RECT rc;
	GetClientRect(hwnd, &rc);						//guarda la resolución de la ventana.

	//se crea adaptadore , vector de adapts y factory para meterlos en el vector
	Vector<IDXGIAdapter1*> vAdapters;
	IDXGIAdapter1* pAdapter = nullptr;
	IDXGIFactory1* pFactory = nullptr;

	CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&pFactory);

	//Se busca adaptadores ocn el factory
	for (UINT i = 0; pFactory->EnumAdapters1(i,&pAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		DXGI_ADAPTER_DESC1 desc;
		pAdapter->GetDesc1(&desc);
		vAdapters.push_back(pAdapter);
	}
	
	SAFE_RELEASE(pAdapter);							//Se libera memoria
	SAFE_RELEASE(pFactory);

													
	Vector<D3D_FEATURE_LEVEL> vFeatureLevels =		//Se enlistan los feature levels que soporte la computadora
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3  
	};

	D3D_FEATURE_LEVEL selectedFeatureLevel;

	//Se colocan los dispositivos
	ID3D11Device* pDevice = nullptr;
	ID3D11DeviceContext* pDeviceContext = nullptr;

	//se ponen flags para que el compilador nos mande errores si hay
	UINT deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	//se crea el device con todo lo anterior
	result = D3D11CreateDevice(	vAdapters[0],
								D3D_DRIVER_TYPE_UNKNOWN,
								nullptr,
								deviceFlags,
								vFeatureLevels.data(),
								vFeatureLevels.size(),
								D3D11_SDK_VERSION,
								&pDevice,
								&selectedFeatureLevel,
								&pDeviceContext);
	if (FAILED(result))	//Si no da el vo bno se va a la goma
	{
		MessageBox(hwnd, L"GraphicsAPI couldn't be created", L"Error", MB_OK);
		return;
	}

	//pedimos interfaces de los devices creados para las variables miembro
	pDevice->QueryInterface(__uuidof(ID3D11Device1), (void**)&m_pDevice);
	pDeviceContext->QueryInterface(__uuidof(ID3D11DeviceContext1), (void**)&m_pDeviceContext);

	//Se liberan
	SAFE_RELEASE(pDeviceContext);
	SAFE_RELEASE(pDevice);

	//Se crea el descriptor de la swap chain
	DXGI_SWAP_CHAIN_DESC1 scDesc;
	memset(&scDesc, 0, sizeof(scDesc));

	scDesc.Width = rc.right;								// tamaño de la ventana
	scDesc.Height = rc.bottom;								//tamaño de la ventana
	scDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;				//fomrato de los pixeles
	scDesc.Stereo = false;									// Para monitores viejos 3d ya no se usa.
	scDesc.SampleDesc.Count = 1;							// Multisample Antialiasing -> MSAA
	scDesc.SampleDesc.Quality = 0;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// Para que se pueda mostrar target render otuputs
	scDesc.BufferCount = 2;									// 2 buffers para que se pueda hacer el swap
	scDesc.Scaling = DXGI_SCALING_NONE;						// No quiero escalados de ningún tipo.
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;	// el efecto de swap que quiero usar

	//Se crean dxgidevices para poder acceder a la función de creación de una swap chain
	IDXGIDevice1* pDXGIDevice = nullptr;					
	m_pDevice->QueryInterface(__uuidof(IDXGIDevice1),(void**)&pDXGIDevice);

	IDXGIAdapter* pDXGIAdapter = nullptr;
	pDXGIDevice->GetAdapter(&pDXGIAdapter);

	IDXGIFactory2* pFactory2 = nullptr;
	pDXGIAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)&pFactory2);

	if (!m_pDevice)
	{
		return;
	}
	//Crear swap chain!
	result = pFactory2->CreateSwapChainForHwnd(	m_pDevice,
												hwnd,
												&scDesc,
												nullptr,
												nullptr,
												&m_pSwapChain);
	pDXGIDevice->SetMaximumFrameLatency(3);

	if (FAILED(result))	//Si no da el vo bno se va a la goma
	{
		MessageBox(hwnd, L"Failed to create swap chain", L"Error", MB_OK);
		return;
	}

	//Hacemos el depth stencil y el render target view
	QueryInterfaces(scDesc.Width,scDesc.Height);

	//Creamos el viewport para que se pueda mostrar información en la pantalla.
	D3D11_VIEWPORT vp;

	vp.Width = static_cast<float>(scDesc.Width);
	vp.Height = static_cast<float>(scDesc.Height);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	m_pDeviceContext->RSSetViewports(1, &vp);

	

}

GraphicsAPI::~GraphicsAPI()
{

}

void GraphicsAPI::QueryInterfaces(int width, int height)
{
	if (!m_pSwapChain)
	{
		return;
	}

	ID3D11Texture2D* pBackBuffer = nullptr;
	m_pSwapChain->GetBuffer(0,__uuidof(ID3D11Texture2D),(void**)&pBackBuffer);
	assert(pBackBuffer);
	m_pDevice->CreateRenderTargetView(pBackBuffer,nullptr,&m_pBackBufferRTV);
	
	SAFE_RELEASE(pBackBuffer);

	ID3D11Texture2D* pDepthStencil = nullptr;
	pDepthStencil = CreateTexture(	width,
									height,
									DXGI_FORMAT_D24_UNORM_S8_UINT,
									D3D11_USAGE_DEFAULT,
									D3D11_BIND_DEPTH_STENCIL);

	if (!pDepthStencil)
	{
		MessageBox(nullptr,L"Failed to create depth stencil",L"Error",MB_OK);
		return;
	}

	m_pDevice->CreateDepthStencilView(pDepthStencil, nullptr, &m_pBackBufferDSV);
	
	SAFE_RELEASE(pDepthStencil);

}

ID3D11Texture2D* GraphicsAPI::CreateTexture(int width, int height, DXGI_FORMAT format, D3D11_USAGE usage, UINT bindFlags,  UINT cpuAccessFlags, UINT mipLevels){
	
	ID3D11Texture2D* pTexture = nullptr;			//Crea un puntero a texturea

	D3D11_TEXTURE2D_DESC desc;
	memset(&desc, 0, sizeof(D3D11_TEXTURE2D_DESC));	//Necesitamos el descriptor

	desc.ArraySize = 1;
	desc.BindFlags = bindFlags;
	desc.CPUAccessFlags = cpuAccessFlags;
	desc.Format = format;
	desc.Height = height;
	desc.Width = width;
	desc.MipLevels = mipLevels;
	desc.MiscFlags = 0;
	desc.SampleDesc.Count = 1; //MSAA
	desc.SampleDesc.Quality = 0; //MSAA
	desc.Usage = usage;

	//Si no lo crea bien, que s evaya por ahí
	if (FAILED(m_pDevice->CreateTexture2D(&desc,nullptr,&pTexture)))
	{
		return nullptr;
	}

	return pTexture;
}

ID3D11VertexShader* GraphicsAPI::CreateVertexShaderOLD(const Path& filePath, const String& entryFunction, const String& shaderModel)
{
	fstream shaderFile(filePath, std::ios::ate | std::ios::in);
	if (!shaderFile.is_open())
	{
		return nullptr;
	}

	//Guardar el tamaño del archivo y luego vuelve al inicio
	size_t fileSize = shaderFile.tellg();
	shaderFile.seekg(0, std::ios::beg);

	//Guardamos los caracteres del código en un vector
	Vector<char> vShaderCode(fileSize);
	shaderFile.read(reinterpret_cast<char*>(vShaderCode.data()),fileSize);

	//Añadimos una flags para el compilador
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(_DEBUG)
	flags |= D3DCOMPILE_DEBUG;
#endif

	//Creamos un blob, que será básicamente un buffer de código objeto
	ID3DBlob* pShaderBlob = nullptr;
	ID3DBlob* pErrorBlob = nullptr;

	HRESULT hr = D3DCompile(vShaderCode.data(),
							vShaderCode.size(),
							nullptr,
							nullptr,
							nullptr,
							entryFunction.c_str(),
							shaderModel.c_str(),
							flags,
							0,
							&pShaderBlob,
							&pErrorBlob);

	if (FAILED(hr))
	{
		MessageBoxA(nullptr, (LPCSTR)pErrorBlob->GetBufferPointer(), "Error", MB_OK);
		SAFE_RELEASE(pErrorBlob);
		return nullptr;
	}

	//Crea el VertexShader con el blob y el tamaño del blob
	ID3D11VertexShader* pVertexShader = nullptr;
	hr = m_pDevice->CreateVertexShader(	pShaderBlob->GetBufferPointer(), 
										pShaderBlob->GetBufferSize(),
										nullptr,
										&pVertexShader);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Failed to create Vertex Shader", L"Error", MB_OK);
		return nullptr;
	}


	return pVertexShader;
}

UPtr<VertexShader> GraphicsAPI::CreateVertexShader(const Path& filePath, const String& entryFunction)
{
	//Crea un puntero al shader y le dice literalemten tu modelo
	UPtr<VertexShader> pVertexShader = make_unique<VertexShader>();
	if (!pVertexShader->Compile(filePath,entryFunction,"vs_5_0"))
	{
		return nullptr;
	}

	//Lo crea con el device y ve si funcionó o no
	HRESULT hr = m_pDevice->CreateVertexShader(	pVertexShader->GetBlob()->GetBufferPointer(),
												pVertexShader->GetBlob()->GetBufferSize(),
												nullptr,
												&pVertexShader->m_pVertexShader);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Failed to create vertex shader", L"Error", MB_OK);
		return nullptr;
	}

	//Lo regresa
	return pVertexShader;
}

UPtr<PixelShader> GraphicsAPI::CreatePixelShader(const Path& filePath, const String& entryFunction)
{
	//Crea un puntero al shader y le dice literalemten tu modelo
	UPtr<PixelShader> pPixelShader = make_unique<PixelShader>();
	if (!pPixelShader->Compile(filePath, entryFunction, "ps_5_0"))
	{
		return nullptr;
	}

	//Lo crea con el device y ve si funcionó o no
	HRESULT hr = m_pDevice->CreatePixelShader(pPixelShader->GetBlob()->GetBufferPointer(),
		pPixelShader->GetBlob()->GetBufferSize(),
		nullptr,
		&pPixelShader->m_pPixelShader);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Failed to create pixel shader", L"Error", MB_OK);
		return nullptr;
	}

	//Lo regresa
	return pPixelShader;
}

ID3D11InputLayout* GraphicsAPI::CreateInputLayout(Vector<D3D11_INPUT_ELEMENT_DESC> pInputElementDesc, const UPtr<VertexShader>& pVertexShader)
{
	ID3D11InputLayout* pInputLayout = nullptr;
	
	// Si el arreglo viene vacío por default, no hagas nada
	if (pInputElementDesc.empty()) {
		return nullptr;
	}

	//Crea el layout con el arreglo de elemntos y el código objeto(blob)
	HRESULT hr = m_pDevice->CreateInputLayout(pInputElementDesc.data(),
								pInputElementDesc.size(),
								pVertexShader->GetBlob()->GetBufferPointer(),
								pVertexShader->GetBlob()->GetBufferSize(),
								&pInputLayout);

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Failed to create input layout", L"Error", MB_OK);
		return nullptr;
	}


	return pInputLayout;
}

UPtr<GraphicsBuffer> GraphicsAPI::CreateVertexBuffer(const Vector<char>& data)
{
	UPtr<GraphicsBuffer> pVertexBuffer = make_unique<GraphicsBuffer>();

	D3D11_BUFFER_DESC desc;
	memset(&desc, 0, sizeof(D3D11_BUFFER_DESC));

	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = data.size();
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = data.data();
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	HRESULT hr = m_pDevice->CreateBuffer(&desc,&initData, &pVertexBuffer->m_pBuffer);

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Failed to create Vertex Buffer", L"Error", MB_OK);
		return nullptr;
	}

	return pVertexBuffer;
}

UPtr<GraphicsBuffer> GraphicsAPI::CreateIndexBuffer(const Vector<char>& data)
{
	UPtr<GraphicsBuffer> pIndexBuffer = make_unique<GraphicsBuffer>();

	D3D11_BUFFER_DESC desc;
	memset(&desc, 0, sizeof(D3D11_BUFFER_DESC));

	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = data.size();
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = data.data();
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	HRESULT hr = m_pDevice->CreateBuffer(&desc, &initData, &pIndexBuffer->m_pBuffer);

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Failed to create Index Buffer", L"Error", MB_OK);
		return nullptr;
	}

	return pIndexBuffer;
}
