#include "Shader.h"

// static data definition
std::map<std::string, VertexShader*> ShaderManager::vertexShaders;
std::map<std::string, PixelShader*> ShaderManager::pixelShaders;

DXCore* ShaderManager::driver = nullptr;
std::string ShaderManager::current[2] = { "\0","\0" };

bool ShaderManager::lockVertex = false;
bool ShaderManager::lockPixel = false;

void ShaderManager::Init(DXCore* _driver)
{
	driver = _driver;
	current[0] = "Default";
	current[1] = "Default";
}

void ShaderManager::AddVertex(std::string _name, std::string _location, LayoutType _type)
{
	if (vertexShaders.find(_name) != vertexShaders.end())
		return;

	VertexShader* shader = new VertexShader(_name, _location, driver, _type);
	vertexShaders.insert({ _name, shader });
}

void ShaderManager::AddPixel(std::string _name, std::string _location)
{
	if (pixelShaders.find(_name) != pixelShaders.end())
		return;

	PixelShader* shader = new PixelShader(_name, _location, driver);
	pixelShaders.insert({ _name, shader });
}

void ShaderManager::SetVertex(std::string _name)
{
	if (lockVertex)
		return;

	if (vertexShaders.find(_name) == vertexShaders.end())
		current[0] = "Default";
	else
		current[0] = _name;
}

void ShaderManager::SetPixel(std::string _name)
{
	if (lockPixel)
		return;

	if (pixelShaders.find(_name) == pixelShaders.end())
		current[1] = "Default";
	else
		current[1] = _name;
}

void ShaderManager::Set(std::string _vName, std::string _pName)
{
	SetVertex(_vName);
	SetPixel(_pName);
}

void ShaderManager::ApplyVertex()
{
	vertexShaders[current[0]]->Apply(driver);
}

void ShaderManager::ApplyPixel()
{
	vertexShaders[current[1]]->Apply(driver);
}

void ShaderManager::Apply()
{
	vertexShaders[current[0]]->Apply(driver);
	pixelShaders[current[1]]->Apply(driver);
}

void ShaderManager::UpdateVertex(std::string _name, std::string _bufferName, std::string _varName, void* _data)
{
	vertexShaders[_name]->UpdateConstant(_bufferName, _varName, _data);
}

void ShaderManager::UpdateVertex(std::string _bufferName, std::string _varName, void* _data)
{
	vertexShaders[current[0]]->UpdateConstant(_bufferName, _varName, _data);
}

void ShaderManager::UpdatePixel(std::string _bufferName, std::string _varName, void* _data)
{
	pixelShaders[current[1]]->UpdateConstant(_bufferName, _varName, _data);
}

void ShaderManager::UpdateAll(ShaderStage _type, std::string _bufferName, std::string _varName, void* _data)
{
	if (ShaderStage::Vertex == _type)
	{
		for (auto& shader : vertexShaders)
			shader.second->UpdateConstant(_bufferName, _varName, _data);
	}
	else
	{
		for (auto& shader : pixelShaders)
			shader.second->UpdateConstant(_bufferName, _varName, _data);
	}
}

void ShaderManager::UpdatePixel(std::string _varName, ID3D11ShaderResourceView* _srv)
{
	pixelShaders[current[1]]->UpdateTexture(_varName, _srv, driver);
}

void ShaderManager::UpdatePixel(std::string _name, std::string _varName, ID3D11ShaderResourceView* _srv)
{
	pixelShaders[_name]->UpdateTexture(_varName, _srv, driver);
}

void ShaderManager::Free()
{
	for (auto data = vertexShaders.cbegin(); data != vertexShaders.cend();)
	{
		delete data->second;
		vertexShaders.erase(data++);
	}

	for (auto data = pixelShaders.cbegin(); data != pixelShaders.cend();)
	{
		delete data->second;
		pixelShaders.erase(data++);
	}

	vertexShaders.clear();
	pixelShaders.clear();

	current[0] = current[1] = "\0";
}
