#pragma once

#define SAFE_RELEASE(p) { if(p){p->Release(); p = nullptr;} }

#include <d3d11.h>
#include <xaudio2.h>
#include "Audio.h"


class Dx11DemoBase
{
public:
	Dx11DemoBase();
	virtual ~Dx11DemoBase();

	bool Intialize(HINSTANCE hInstance, HWND hwnd);
	void Shutdown();

	virtual bool LoadContent();
	virtual void UnloadContent();

	virtual void Update(float dt) = 0;
	virtual void Render() = 0;

protected:
	//Windows stuff
	HINSTANCE m_hInstance;
	HWND m_hwnd;
	
	//D3D Stuff
	D3D_DRIVER_TYPE m_driverType;
	D3D_FEATURE_LEVEL m_featureLevel;
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;
	IDXGISwapChain* m_pSwapChain = nullptr;
	ID3D11RenderTargetView* m_pBackBufferRTV = nullptr;
	
	//Audio
	IXAudio2* m_pXAudio2 = nullptr;
	
	
	IXAudio2MasteringVoice* m_pXAudio2MasteringVoice = nullptr;
	IXAudio2SourceVoice* m_pXAudio2SourceVoice = nullptr;
	Audio m_audioFile;
	Audio m_audioPrueba;

};

