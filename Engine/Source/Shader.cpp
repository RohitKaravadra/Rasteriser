#pragma once
#include "Shader.h"
#include <sstream>
#include "Utilities.h"
#include <codecvt>

#pragma region Shader Rflection

void ConstantBuffer::init(DXCore& _driver, unsigned int sizeInBytes, int constantBufferIndex, ShaderStage _shaderStage)
{
	unsigned int sizeInBytes16 = ((sizeInBytes + 15) & -16);
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA data;
	bd.ByteWidth = sizeInBytes16;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	_driver.device->CreateBuffer(&bd, NULL, &cb);
	buffer = new unsigned char[sizeInBytes16];
	cbSizeInBytes = sizeInBytes;
	index = constantBufferIndex;
	dirty = 1;
	shaderStage = _shaderStage;
}

void  ConstantBuffer::update(std::string name, void* data)
{
	ConstantBufferVariable cbVariable = constantBufferData[name];
	memcpy(&buffer[cbVariable.offset], data, cbVariable.size);
	dirty = 1;
}

void  ConstantBuffer::upload(DXCore& _driver)
{
	if (dirty == 1)
	{
		D3D11_MAPPED_SUBRESOURCE mapped;
		_driver.devicecontext->Map(cb, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
		memcpy(mapped.pData, buffer, cbSizeInBytes);
		_driver.devicecontext->Unmap(cb, 0);
		if (shaderStage == ShaderStage::VertexShader)
		{
			_driver.devicecontext->VSSetConstantBuffers(index, 1, &cb);
		}
		if (shaderStage == ShaderStage::PixelShader)
		{
			_driver.devicecontext->PSSetConstantBuffers(index, 1, &cb);
		}
		dirty = 0;
	}
}
void  ConstantBuffer::free()
{
	cb->Release();
}


void ConstantBufferReflection::build(DXCore& _driver, ID3DBlob* shader, std::vector<ConstantBuffer>& buffers, std::map<std::string, int>& textureBindPoints, ShaderStage shaderStage)
{
	ID3D11ShaderReflection* reflection;
	D3DReflect(shader->GetBufferPointer(), shader->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&reflection);
	D3D11_SHADER_DESC desc;
	reflection->GetDesc(&desc);
	for (int i = 0; i < desc.ConstantBuffers; i++)
	{
		ConstantBuffer buffer;
		ID3D11ShaderReflectionConstantBuffer* constantBuffer = reflection->GetConstantBufferByIndex(i);
		D3D11_SHADER_BUFFER_DESC cbDesc;
		constantBuffer->GetDesc(&cbDesc);
		buffer.name = cbDesc.Name;
		unsigned int totalSize = 0;
		for (int n = 0; n < cbDesc.Variables; n++)
		{
			ID3D11ShaderReflectionVariable* var = constantBuffer->GetVariableByIndex(n);
			D3D11_SHADER_VARIABLE_DESC vDesc;
			var->GetDesc(&vDesc);
			ConstantBufferVariable bufferVariable;
			bufferVariable.offset = vDesc.StartOffset;
			bufferVariable.size = vDesc.Size;
			buffer.constantBufferData.insert({ vDesc.Name, bufferVariable });
			totalSize += bufferVariable.size;
		}
		buffer.init(_driver, totalSize, i, shaderStage);
		buffers.push_back(buffer);
	}

	// for textures
	for (int i = 0; i < desc.BoundResources; i++)
	{
		D3D11_SHADER_INPUT_BIND_DESC bindDesc;
		reflection->GetResourceBindingDesc(i, &bindDesc);
		if (bindDesc.Type == D3D_SIT_TEXTURE)
		{
			textureBindPoints.insert({ bindDesc.Name, bindDesc.BindPoint });
		}
	}
}

#pragma endregion

#pragma region Shader

Shader::Shader(std::string _name, std::string _vsLocation, std::string _psLocation, DXCore& _driver, ShaderType _type)
{
	name = _name;

	// compile shaders
	CompileVertexShader(_vsLocation, _type, _driver);
	CompilePixelShader(_psLocation, _driver);
}

void Shader::BuildLayout(ShaderType _type, ID3DBlob* _vertexShader, DXCore& _driver)
{
	// create layout
	if (_type == ShaderType::Animated)
	{	// set layout
		D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
		{
			{ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BONEIDS", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		// create layout
		_driver.device->CreateInputLayout(layoutDesc, 6, _vertexShader->GetBufferPointer(), _vertexShader->GetBufferSize(), &layout);
	}
	else if (_type == ShaderType::Instancing)
	{
		D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
		{
			{ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "INSTANCEPOSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		};

		// create layout
		_driver.device->CreateInputLayout(layoutDesc, 5, _vertexShader->GetBufferPointer(), _vertexShader->GetBufferSize(), &layout);
	}
	else
	{
		// set layout
		D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
		{
			{ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		// create layout
		_driver.device->CreateInputLayout(layoutDesc, 4, _vertexShader->GetBufferPointer(), _vertexShader->GetBufferSize(), &layout);
	}
}

void Shader::CompileVertexShader(std::string _location, ShaderType _type, DXCore& _driver)
{
	ID3DBlob* compiledShader; // store compiled vertex shader

	std::string shaderName = ExtractName(_location);
	ReplaceString(shaderName, ".hlsl", ".cso");
	std::wstring path = L"Cache/" + std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(shaderName);

	HRESULT hr = D3DReadFileToBlob(path.c_str(), &compiledShader); // load if already compiled
	if (FAILED(hr))
	{
		std::string shaderData = GetFileData(_location);
		ID3DBlob* status; // store compilation message
		HRESULT hr = D3DCompile(shaderData.c_str(), strlen(shaderData.c_str()), NULL, NULL, NULL, "Vertex", "vs_5_0", 0, 0, &compiledShader, &status);
		if (FAILED(hr)) // check for failure
		{
			MessageBoxA(NULL, (char*)status->GetBufferPointer(), "Vertex Shader Error", 0);
			exit(0);
		}
		D3DWriteBlobToFile(compiledShader, path.c_str(), false);
	}

	// create and store vertex shader
	_driver.device->CreateVertexShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), NULL, &vertexShader);

	BuildLayout(_type, compiledShader, _driver);

	// create Constant buffer
	ConstantBufferReflection reflection;
	reflection.build(_driver, compiledShader, vsConstantBuffers, textureBindPointsVS, ShaderStage::VertexShader);
}

void Shader::CompilePixelShader(std::string _location, DXCore& _driver)
{
	ID3DBlob* compiledShader; // store compiled pixel shader

	std::string shaderName = ExtractName(_location);
	ReplaceString(shaderName, ".hlsl", ".cso");
	std::wstring path = L"Cache/" + std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(shaderName);

	HRESULT hr = D3DReadFileToBlob(path.c_str(), &compiledShader);// load if already compiled
	if (FAILED(hr))
	{
		std::string shaderData = GetFileData(_location);
		ID3DBlob* status; // store of the compilation message
		HRESULT hr = D3DCompile(shaderData.c_str(), strlen(shaderData.c_str()), NULL, NULL, NULL, "Pixel", "ps_5_0", 0, 0, &compiledShader, &status);
		if (FAILED(hr)) // check of failure
		{
			MessageBoxA(NULL, (char*)status->GetBufferPointer(), "Pixel Shader Error", 0);
			exit(0);
		}
		D3DWriteBlobToFile(compiledShader, path.c_str(), false);

	}

	// create and store pixel shader
	_driver.device->CreatePixelShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), NULL, &pixelShader);

	// create constant buffer
	ConstantBufferReflection reflection;
	reflection.build(_driver, compiledShader, psConstantBuffers, textureBindPointsPS, ShaderStage::PixelShader);
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

void Shader::UpdateConstant(ShaderStage _type, std::string constantBufferName, std::string variableName, void* data)
{
	UpdateConstant(constantBufferName, variableName, data, _type == ShaderStage::VertexShader ? vsConstantBuffers : psConstantBuffers);
}

void Shader::UpdateTexture(ShaderStage _type, std::string _name, ID3D11ShaderResourceView* srv, DXCore& _driver)
{
	if (_type == ShaderStage::PixelShader)
		_driver.devicecontext->PSSetShaderResources(textureBindPointsPS[_name], 1, &srv);
	else
		_driver.devicecontext->VSSetShaderResources(textureBindPointsPS[_name], 1, &srv);
}

Shader::~Shader()
{
	if (vertexShader != nullptr)
		vertexShader->Release();
	if (pixelShader != nullptr)
		pixelShader->Release();
	if (layout != nullptr)
		layout->Release();
}

#pragma endregion

#pragma region Shader Manager

std::map<std::string, Shader*> ShaderManager::shaders; // store shaders
DXCore* ShaderManager::driver = nullptr; // reference to the device
std::string ShaderManager::current = "\0"; // current applied shader

void ShaderManager::Init(DXCore* _driver)
{
	driver = _driver;
	current = "Default";
}

void ShaderManager::Add(std::string _name, std::string _vsLocation, std::string _psLocation, ShaderType _type)
{
	if (shaders.find(_name) != shaders.end())
		return;

	Shader* shader = new Shader(_name, _vsLocation, _psLocation, *driver, _type);
	shaders.insert({ _name, shader });
}

void ShaderManager::Set(std::string _name)
{
	if (shaders.find(_name) == shaders.end())
		current = "Default";
	else
		current = _name;
}

void ShaderManager::Apply()
{

	if (driver == nullptr)
		return;

	shaders[current]->Apply(*driver);
}

void ShaderManager::UpdateConstant(ShaderStage _type, std::string constantBufferName, std::string variableName, void* data)
{
	shaders[current]->UpdateConstant(_type, constantBufferName, variableName, data);
}

void ShaderManager::UpdateConstant(std::string _name, ShaderStage _type, std::string constantBufferName, std::string variableName, void* data)
{
	shaders[_name]->UpdateConstant(_type, constantBufferName, variableName, data);
}

// do not use yet (need to update according to the name of constantBuffer)
void ShaderManager::UpdateConstantForAll(ShaderStage _type, std::string constantBufferName, std::string variableName, void* data)
{
	for (auto& shader : shaders)
		shader.second->UpdateConstant(_type, constantBufferName, variableName, data);
}

// update texture
void ShaderManager::UpdateTexture(ShaderStage _type, std::string _name, ID3D11ShaderResourceView* srv)
{
	shaders[current]->UpdateTexture(_type, _name, srv, *driver);
}

// update texture
void ShaderManager::UpdateTexture(std::string _shader, ShaderStage _type, std::string _name, ID3D11ShaderResourceView* srv)
{
	shaders[_shader]->UpdateTexture(_type, _name, srv, *driver);
}

void ShaderManager::Free()
{
	for (auto data = shaders.cbegin(); data != shaders.cend();)
	{
		delete data->second;
		shaders.erase(data++);
	}
}

#pragma endregion
