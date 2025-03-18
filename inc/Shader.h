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
Un shader será un programa(código) que se usará para crear el
VertexShader, el código se guardará en un Blob, que es un
espacio en memoria de puro código objeto, después de la
compilación de d3dcompiler. Se usará para tener más hijos de
shader que serán los vertex shader, el pixel shader y así.
*/

class Shader
{
public:
	Shader() = default;
	virtual ~Shader() {
		SAFE_RELEASE(m_pBlob);
	}

	//Compilaremos el código en HLSL
	bool Compile(const Path& filePath,
				const String& entryFunction,
				const String& shaderModel);

	ID3DBlob* GetBlob() const {
		return m_pBlob;
	}

private:
	//Código objeto del programa en HLSL
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
