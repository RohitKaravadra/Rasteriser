#include "FullScreenQuad.h"
#include "Utilities.h"
#include <codecvt>

FullScreenQuad::FullScreenQuad(std::string _vsLocation, std::string _psLocation, DXCore* _driver)
{
	// compile shaders
	CompileVertexShader(_vsLocation, *_driver);
	CompilePixelShader(_psLocation, *_driver);
}

void FullScreenQuad::CompileVertexShader(std::string _location, DXCore& _driver)
{
	ID3DBlob* compiledShader; // store compiled vertex shader

	std::string shaderName = ExtractName(_location);
	ReplaceString(shaderName, ".hlsl", ".cso");
	std::string tmp;
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
}

void FullScreenQuad::CompilePixelShader(std::string _location, DXCore& _driver)
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

void FullScreenQuad::Apply(DXCore* _driver)
{
	_driver->devicecontext->IASetInputLayout(NULL); // set layout

	_driver->devicecontext->VSSetShader(vertexShader, NULL, 0); // apply vertex shader
	_driver->devicecontext->PSSetShader(pixelShader, NULL, 0); // apply pixel shader
}

void FullScreenQuad::DrawTexture(std::string _name, ID3D11ShaderResourceView* srv, DXCore* _driver)
{
	_driver->devicecontext->PSSetShaderResources(textureBindPointsPS[_name], 1, &srv);
	Apply(_driver);
	_driver->devicecontext->Draw(3, 0);
}

FullScreenQuad::~FullScreenQuad()
{
	vertexShader->Release();
	pixelShader->Release();
}
