#pragma once
#define WIN32_LEAN_AND_MEAN
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <Windows.h>
#include "HelperMacros.h"
#include "GraphicsAPI.h"

class ImGuiAPI
{
public:
	ImGuiAPI();
	~ImGuiAPI();

	bool Init(void* hwnd, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	void Update();
	void Render();

protected:
	HWND m_hwnd;
};

