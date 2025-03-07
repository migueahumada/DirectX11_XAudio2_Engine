#pragma once
#include <d3d11_2.h> 
#include "HelperMacros.h"

class GraphicsBuffer
{
public:
	GraphicsBuffer() = default;
	virtual ~GraphicsBuffer() {
		SAFE_RELEASE(m_pBuffer);
	}


	ID3D11Buffer* m_pBuffer = nullptr;
};

