#pragma once
#include "Shader.h"
#include <sstream>

static std::string GetFileData(std::string _fileName)
{
	std::ifstream file(_fileName);
	if (file)
	{
		std::stringstream data;
		data << file.rdbuf();
		return data.str();
	}
	else
	{
		std::string msg = "File " + _fileName + " Not Found";
		MessageBoxA(NULL, msg.c_str(), "Vertex Shader Error", 0);
		exit(0);
	}
	return "";
}

#pragma region Shader

void Shader::Init(std::string _vsLocation, std::string _psLocation, DXCore& _driver)
{
	// get shaders
	std::string vertexS = GetFileData(_vsLocation);
	std::string pixelS = GetFileData(_psLocation);

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

void Shader::Apply(DXCore& _driver)
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

void Shader::UpdateConstant(std::string constantBufferName, std::string variableName, void* data, std::vector<ConstantBuffer>& buffers)
{
	for (int i = 0; i < buffers.size(); i++)
	{
		if (buffers[i].name == constantBufferName)
		{
			buffers[i].update(variableName, data);
			return;
		}
	}
}

// update vertex constant buffer
void Shader::UpdateConstant(ShaderType _type, std::string constantBufferName, std::string variableName, void* data)
{
	UpdateConstant(constantBufferName, variableName, data, _type == ShaderType::Vertex ? vsConstantBuffers : psConstantBuffers);
}

#pragma endregion

#pragma region Shader Manager

std::map<std::string, Shader> ShaderManager::shaders; // store shaders
DXCore* ShaderManager::driver = nullptr; // reference to the device
std::string ShaderManager::current = "\0"; // current applied shader

void ShaderManager::SetDevice(DXCore& _driver)
{
	driver = &_driver;
}

void ShaderManager::Add(std::string _name, std::string _vsLocation, std::string _psLocation)
{
	if (shaders.find(_name) != shaders.end())
		return;

	Shader shader;
	shader.Init(_vsLocation, _psLocation, *driver);
	shaders.insert({ _name, shader });
}

void ShaderManager::Apply(std::string _name)
{
	/*if (_current == _name)
		return;*/
	if (driver == nullptr || shaders.find(_name) == shaders.end())
		return;

	current = _name;
	shaders[_name].Apply(*driver);
}

void ShaderManager::UpdateConstant(std::string _name, ShaderType _type, std::string constantBufferName, std::string variableName, void* data)
{
	if (shaders.find(_name) == shaders.end())
		return;

	shaders[_name].UpdateConstant(_type, constantBufferName, variableName, data);
}

#pragma endregion
