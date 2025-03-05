#include "BlankDemo.h"

BlankDemo::BlankDemo()
{
}

BlankDemo::~BlankDemo()
{
}

bool BlankDemo::LoadContent()
{




	return true;
}

void BlankDemo::UnloadContent()
{

}

void BlankDemo::Update(float dt)
{
}

void BlankDemo::Render()
{
	if (!m_pDeviceContext)
		return;

	float clearColor[4] = { 0.4f, 0.4f, 0.25f, 1.0f };
	m_pDeviceContext->ClearRenderTargetView(m_pBackBufferRTV,clearColor);

	m_pSwapChain->Present(0, 0);
}
