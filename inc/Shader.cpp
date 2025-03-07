#include "Shader.h"

bool Shader::Compile(const Path& filePath, const String& entryFunction, const String& shaderModel)
{
	fstream shaderFile(filePath, std::ios::ate | std::ios::in);
	if (!shaderFile.is_open())
	{
		return false;
	}

	//Guardar el tamaño del archivo y luego vuelve al inicio
	size_t fileSize = shaderFile.tellg();
	shaderFile.seekg(0, std::ios::beg);

	//Guardamos los caracteres del código en un vector
	Vector<char> vShaderCode(fileSize);
	shaderFile.read(reinterpret_cast<char*>(vShaderCode.data()), fileSize);

	//Añadimos una flags para el compilador
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(_DEBUG)
	flags |= D3DCOMPILE_DEBUG;
#endif

	//Creamos un blob, que será básicamente un buffer de código objeto
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
		&m_pBlob,
		&pErrorBlob);

	if (FAILED(hr))
	{
		MessageBoxA(nullptr, (LPCSTR)pErrorBlob->GetBufferPointer(), "Error", MB_OK);
		SAFE_RELEASE(pErrorBlob);
		return false;
	}

	return true;
}
