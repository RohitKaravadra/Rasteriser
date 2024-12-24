#pragma once
#include "../hdr/Shader.h"
#include "../hdr/Utilities.h"

// function checks and load shader if available
// compile and save shader if not available
// parameter load is for testing to compile shader everytime
ID3DBlob* GetCompiled(ShaderStage _stage, std::string _location, bool _load)
{
	ID3DBlob* compiledShader;

	// create a compiled path
	std::string shaderName = ExtractName(_location);
	ReplaceString(shaderName, ".hlsl", ".cso");
	std::wstring path = StringToWstring("Cache/" + shaderName);

	// load compiled shader if available
	if (_load)
	{
		HRESULT hr = D3DReadFileToBlob(path.c_str(), &compiledShader); // load if already compiled
		if (SUCCEEDED(hr))
			return compiledShader;
	}

	// get shader data from shader file
	std::string shaderData = GetFileData(_location);
	ID3DBlob* status; // store compilation message
	HRESULT hr;

	// compile shader according to the shader stage
	if (ShaderStage::Vertex == _stage)
		hr = D3DCompile(shaderData.c_str(), strlen(shaderData.c_str()), NULL, NULL, NULL, "Vertex", "vs_5_0", 0, 0, &compiledShader, &status);
	else
		hr = D3DCompile(shaderData.c_str(), strlen(shaderData.c_str()), NULL, NULL, NULL, "Pixel", "ps_5_0", 0, 0, &compiledShader, &status);

	// check result and display error if any
	if (FAILED(hr)) // check for failure
	{
		if (ShaderStage::Vertex == _stage)
			MessageBoxA(NULL, (char*)status->GetBufferPointer(), "Vertex Shader Error", 0);
		else
			MessageBoxA(NULL, (char*)status->GetBufferPointer(), "Pixel Shader Error", 0);

		exit(0);
	}

	// Save compiled shader
	D3DWriteBlobToFile(compiledShader, path.c_str(), false);

	return compiledShader;
}

VertexShader::VertexShader(std::string _name, std::string _location, DXCore* _driver, LayoutType _type)
{
	name = _name;
	layout = NULL; // initial value for layout
	CompileVertexShader(_location, _type, _driver);
}

void VertexShader::BuildLayout(LayoutType _type, ID3DBlob* _shader, DXCore* _driver)
{
	D3D11_INPUT_ELEMENT_DESC* layoutDesc;
	int size;

	// select size and extra info
	switch (_type)
	{
		// case fore no layout (usually its only possible if logical error in code)
	case LayoutType::None:
		size = 0;
		return;

	case LayoutType::Normal:
		size = 4;
		layoutDesc = new D3D11_INPUT_ELEMENT_DESC[size]();
		break;
	case LayoutType::Animated:
		size = 6;
		layoutDesc = new D3D11_INPUT_ELEMENT_DESC[size]();
		layoutDesc[4] = { "BONEIDS", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 };
		layoutDesc[5] = { "BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 };
		break;
	case LayoutType::Instancing:
		size = 5;
		layoutDesc = new D3D11_INPUT_ELEMENT_DESC[size]();
		layoutDesc[4] = { "INSTANCEPOSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 };
		break;

	default:
		return;
	}

	// default info
	layoutDesc[0] = { "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 };
	layoutDesc[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 };
	layoutDesc[2] = { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 };
	layoutDesc[3] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 };

	// create layout
	_driver->device->CreateInputLayout(layoutDesc, size, _shader->GetBufferPointer(), _shader->GetBufferSize(), &layout);
	delete[] layoutDesc;
}

void VertexShader::CompileVertexShader(std::string _location, LayoutType _type, DXCore* _driver)
{
	ID3DBlob* compiledShader = GetCompiled(ShaderStage::Vertex, _location);// store compiled vertex shader

	// create and store vertex shader
	_driver->device->CreateVertexShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), NULL, &shader);

	// no need to build shader reflection if no layout
	if (_type == LayoutType::None)
	{
		compiledShader->Release();
		return;
	}

	BuildLayout(_type, compiledShader, _driver);

	// create Constant buffer
	ConstantBufferReflection reflection;
	reflection.build(*_driver, compiledShader, constBuffer, texBindPoints, ShaderStage::Vertex);

	compiledShader->Release();
}

void VertexShader::Apply(DXCore* _driver)
{
	_driver->devContext->IASetInputLayout(layout); // set layout
	_driver->devContext->VSSetShader(shader, NULL, 0); // apply vertex shader

	// set constant buffer in pixel shader
	for (int i = 0; i < constBuffer.size(); i++)
		constBuffer[i].upload(*_driver);
}

void VertexShader::UpdateConstant(std::string _bufferName, std::string _varName, void* _data)
{
	for (int i = 0; i < constBuffer.size(); i++)
	{
		if (constBuffer[i].name == _bufferName)
		{
			constBuffer[i].update(_varName, _data);
			return;
		}
	}
}

VertexShader::~VertexShader()
{
	if (shader != nullptr)
		shader->Release();
	if (layout != nullptr)
		layout->Release();

	constBuffer.clear();
	texBindPoints.clear();
}

PixelShader::PixelShader(std::string _name, std::string _location, DXCore* _driver)
{
	name = _name;
	CompilePixelShader(_location, _driver);
}

void PixelShader::CompilePixelShader(std::string _location, DXCore* _driver)
{
	ID3DBlob* compiledShader = GetCompiled(ShaderStage::Pixel, _location); // store compiled pixel shader

	// create and store pixel shader
	_driver->device->CreatePixelShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), NULL, &shader);

	// create constant buffer
	ConstantBufferReflection reflection;
	reflection.build(*_driver, compiledShader, constBuffer, texBindPoints, ShaderStage::Pixel);

	compiledShader->Release();
}

void PixelShader::Apply(DXCore* _driver)
{
	_driver->devContext->PSSetShader(shader, NULL, 0); // apply pixel shader

	// set constant buffer in pixel shader
	for (int i = 0; i < constBuffer.size(); i++)
		constBuffer[i].upload(*_driver);
}

void PixelShader::UpdateConstant(std::string _bufferName, std::string _varName, void* _data)
{
	for (int i = 0; i < constBuffer.size(); i++)
	{
		if (constBuffer[i].name == _bufferName)
		{
			constBuffer[i].update(_varName, _data);
			return;
		}
	}
}

void PixelShader::UpdateTexture(std::string _name, ID3D11ShaderResourceView* _srv, DXCore* _driver)
{
	_driver->devContext->PSSetShaderResources(texBindPoints[_name], 1, &_srv);
}

PixelShader::~PixelShader()
{
	if (shader != nullptr)
		shader->Release();

	constBuffer.clear();
	texBindPoints.clear();
}
