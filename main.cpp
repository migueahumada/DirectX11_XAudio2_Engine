
#include <Windows.h>
#include <memory>
#include "BlankDemo.h"

#define WIDTH 1280
#define HEIGHT 720

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE prevInstance,
					_In_ LPWSTR cmdLine, _In_ int cmdShow) {

	UNREFERENCED_PARAMETER(prevInstance);
	UNREFERENCED_PARAMETER(cmdLine);

	WNDCLASSEX wndClass;											//Struct for window creation
	memset(&wndClass,0,sizeof(WNDCLASSEX));

	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW; //Window styles
	wndClass.lpfnWndProc = WndProc;
	wndClass.hInstance = hInstance;
	wndClass.hCursor = LoadCursor(nullptr,IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndClass.lpszMenuName = nullptr;
	wndClass.lpszClassName = L"DX11BookWindowClass";

	if (!RegisterClassEx(&wndClass))
		return -1;

	RECT rc = {0,0,WIDTH,HEIGHT };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	HWND hwnd = CreateWindowA("DX11BookWindowClass",
		"Blank Win 32 Window", 
		WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	if (!hwnd)
	{
		return -1;
	}

	ShowWindow(hwnd, cmdShow);

	auto demo = std::make_shared<BlankDemo>();

	bool result = demo->Intialize(hInstance, hwnd);
	if (!result)
	{
		return -1;
	}

	MSG msg = {0};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg,0,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			demo->Update(0.0f);
			demo->Render();
		}
	}

	demo->Shutdown();

	return static_cast<int>(msg.wParam);

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT paintStruct;
	HDC hDC;

	switch (message)
	{
	case WM_PAINT:
		hDC = BeginPaint(hwnd, &paintStruct);
		EndPaint(hwnd, &paintStruct);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
}