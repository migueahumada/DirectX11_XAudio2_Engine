#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <d3dcompiler.h>
#include <d3d11_2.h>
#include "HelperMacros.h"

template <typename T>
using Vector = std::vector<T>;

using Path = std::wstring;
using String = std::string;
using std::fstream;

/*
Un shader ser� un programa(c�digo) que se usar� para crear el
VertexShader, el c�digo se guardar� en un Blob, que es un
espacio en memoria de puro c�digo objeto, despu�s de la
compilaci�n de d3dcompiler. Se usar� para tener m�s hijos de
shader que ser�n los vertex shader, el pixel shader y as�.
*/

class Shader
{
public:
	Shader() = default;
	virtual ~Shader() {
		SAFE_RELEASE(m_pBlob);
	}

	//Compilaremos el c�digo en HLSL
	bool Compile(const Path& filePath,
				const String& entryFunction,
				const String& shaderModel);

	ID3DBlob* GetBlob() const {
		return m_pBlob;
	}

private:
	//C�digo objeto del programa en HLSL
	ID3DBlob* m_pBlob = nullptr;
};

class VertexShader : public Shader {

public:
	VertexShader() = default;
	virtual ~VertexShader() {
		SAFE_RELEASE(m_pVertexShader);
	}

	friend class GraphicsAPI;
	ID3D11VertexShader* m_pVertexShader = nullptr;
};

class PixelShader : public Shader {

public:
	PixelShader() = default;
	virtual ~PixelShader() {
		SAFE_RELEASE(m_pPixelShader);
	}
	friend class GraphicsAPI;
	ID3D11PixelShader* m_pPixelShader = nullptr;
};
