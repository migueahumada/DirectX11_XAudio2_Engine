#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "GraphicsAPI.h"
#include "AudioAPI.h"
#include "ImGuiAPI.h"


/*
	Estructura que se usará para pasar datos del HLSL a DX11
*/
struct MODEL_VERTEX {
	//POSITION
	float x;
	float y;
	float z;
	//COLOR
	float r;
	float g;
	float b;
};

class BaseGame
{
public:
	BaseGame();
	virtual ~BaseGame();

	bool Init(HINSTANCE hInstance, HWND hwnd);
	void Shutdown();

	virtual bool LoadContent();
	virtual void UnloadContent();

	virtual void Update(float dt);
	virtual void Render();

//protected:
	//Windows stuff
	HINSTANCE m_hInstance = nullptr;
	HWND m_hwnd = nullptr;
	UPtr<GraphicsAPI> m_pGraphicsAPI;
	UPtr<AudioAPI> m_pAudioAPI;
	UPtr<ImGuiAPI> m_pImGuiAPI;

	UPtr<VertexShader> m_pVertexShader;
	UPtr<PixelShader> m_pPixelShader;

	ID3D11InputLayout* m_pInputLayout = nullptr;

	UPtr<GraphicsBuffer> m_pVertexBuffer;

	SPtr<Audio> pSound;
	SPtr<Channel> pChannel;

};

