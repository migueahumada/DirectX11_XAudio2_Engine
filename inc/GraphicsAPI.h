#pragma once

#include <d3d11_2.h>
#include <d3dcompiler.h>
#include <vector>
#include <cassert>
#include <cstring>
#include <fstream>
#include "Shader.h"
#include "GraphicsBuffer.h"
#include "HelperMacros.h"

template <typename T>
using Vector = std::vector<T>;

using Path = std::wstring;
using String = std::string;

using std::fstream;

class GraphicsAPI
{
public:
	GraphicsAPI(void* pWindowHandle);
	virtual ~GraphicsAPI();

	//Creates render target and depth stencil for the backbuffer.
	void QueryInterfaces(int width, int height);

	/*
		Creates a pointer to a texture
		- format: RGB format
		- bindFlags: what use the texture has?
	*/
	ID3D11Texture2D* CreateTexture(
		int width,
		int height,
		DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM,
		D3D11_USAGE usage = D3D11_USAGE_DEFAULT,
		UINT bindFlags = D3D11_USAGE_DEFAULT,
		UINT cpuAccessFlags = 0,
		UINT mipLevels = 0);



	/*Manera anterior de crear un pixel shader, no está abstraida*/
	ID3D11VertexShader* CreateVertexShaderOLD(	const Path& filePath, 
												const String& entryFunction,
												const String& shaderModel);
	
	/*Crea un vertex shader con la ruta del archivo y su función de entrada*/
	UPtr<VertexShader> CreateVertexShader(	const Path& filePath,
											const String& entryFunction);
	
	/*Crea un pixel shader con la ruta del archivo y su función de entrada*/
	UPtr<PixelShader> CreatePixelShader(const Path& filePath,
										const String& entryFunction);

	/*
	Crea una distribución de cómo se pasará la información del shader
	a la tarjeta gráfica y qué modificará.
	*/
	ID3D11InputLayout* CreateInputLayout(Vector<D3D11_INPUT_ELEMENT_DESC> pInputElementDesc,
										 const UPtr<VertexShader>& pVertexShader);
	
	UPtr<GraphicsBuffer> CreateVertexBuffer(const Vector<char>& data);
	UPtr<GraphicsBuffer> CreateIndexBuffer(const Vector<char>& data);

public:
	void* m_pHwnd = nullptr;

	
	ID3D11Device1* m_pDevice = nullptr;					//Video Device
	ID3D11DeviceContext1* m_pDeviceContext = nullptr;	//Video device context
	//Double buffer for presenting stuff in screen
	IDXGISwapChain1* m_pSwapChain = nullptr;

	ID3D11RenderTargetView* m_pBackBufferRTV = nullptr;
	ID3D11DepthStencilView* m_pBackBufferDSV = nullptr;

	//ID3D11VertexShader* m_pVertexShader = nullptr;
};

