#pragma once
#include "Shader.h"

static std::string GetFileData(std::string fileName)
{
	std::ifstream file(fileName);
	if (file)
	{
		std::stringstream data;
		data << file.rdbuf();
		return data.str();
	}
	else
		exit(0);
	return "";
}

#pragma region Shader

void Shader::Init(std::string _VSLocation, std::string _PSLocation, DXCore& _driver)
{
	// get shaders
	std::string vertexS = GetFileData(_VSLocation);
	std::string pixelS = GetFileData(_PSLocation);

	// compile shaders
	CompileVertexShader(vertexS, _driver);
	CompilePixelShader(pixelS, _driver);
}

void Shader::CompileVertexShader(std::string _shader, DXCore& _driver)
{
	ID3DBlob* compiledVertexShader; // store compiled vertex shader
	ID3DBlob* status; // store compilation message
	HRESULT hr = D3DCompile(_shader.c_str(), strlen(_shader.c_str()), NULL, NULL, NULL, "Vertex", "vs_5_0", 0, 0, &compiledVertexShader, &status);
	if (FAILED(hr)) // check for failure
	{
		MessageBoxA(NULL, (char*)status->GetBufferPointer(), "Vertex Shader Error", 0);
		exit(0);
	}
	// create and store vertex shader
	_driver.device->CreateVertexShader(compiledVertexShader->GetBufferPointer(), compiledVertexShader->GetBufferSize(), NULL, &vertexShader);

	// set layout
	D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
	{
		{ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	/*D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
	{
		{ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};*/

	// create layout
	_driver.device->CreateInputLayout(layoutDesc, 4, compiledVertexShader->GetBufferPointer(), compiledVertexShader->GetBufferSize(), &layout);

	// create Constant buffer
	ConstantBufferReflection reflection;
	reflection.build(_driver, compiledVertexShader, vsConstantBuffers, textureBindPointsVS, ShaderStage::VertexShader);
}

void Shader::CompilePixelShader(std::string _shader, DXCore& _driver)
{
	ID3DBlob* compiledPixelShader; // store compiled pixel shader
	ID3DBlob* status; // store of the compilation message
	HRESULT hr = D3DCompile(_shader.c_str(), strlen(_shader.c_str()), NULL, NULL, NULL, "Pixel", "ps_5_0", 0, 0, &compiledPixelShader, &status);
	if (FAILED(hr)) // check of failure
	{
		MessageBoxA(NULL, (char*)status->GetBufferPointer(), "Pixel Shader Error", 0);
		exit(0);
	}

	// create and store pixel shader
	_driver.device->CreatePixelShader(compiledPixelShader->GetBufferPointer(), compiledPixelShader->GetBufferSize(), NULL, &pixelShader);

	// create constant buffer
	ConstantBufferReflection reflection;
	reflection.build(_driver, compiledPixelShader, psConstantBuffers, textureBindPointsPS, ShaderStage::PixelShader);
}

void Shader::ApplyShader(DXCore& _driver)
{
	_driver.devicecontext->IASetInputLayout(layout); // set layout

	_driver.devicecontext->VSSetShader(vertexShader, NULL, 0); // apply vertex shader
	_driver.devicecontext->PSSetShader(pixelShader, NULL, 0); // apply pixel shader

	// set constant buffer in pixel shader
	for (int i = 0; i < vsConstantBuffers.size(); i++)
		vsConstantBuffers[i].upload(_driver);
	for (int i = 0; i < psConstantBuffers.size(); i++)
		psConstantBuffers[i].upload(_driver);
}

#pragma endregion

#pragma region Shader Manager

std::map<std::string, Shader> ShaderManager::shaders;

void ShaderManager::Load(std::vector<std::string>& _shaders, DXCore& _driver)
{
	int size = _shaders.size() / 3;
	for (int i = 0; i < size; i += 3)
	{
		Shader shader;
		shader.Init(_shaders[i + 1], _shaders[i + 2], _driver);
		shaders[_shaders[i]] = shader;
	}
}

bool ShaderManager::GetShader(std::string _name, Shader& _shader)
{
	if (shaders.find(_name) == shaders.end())
		return false;

	_shader = shaders.at(_name);
	return true;
}

#pragma endregion
