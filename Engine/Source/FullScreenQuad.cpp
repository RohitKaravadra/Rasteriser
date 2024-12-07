#include "FullScreenQuad.h"

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

FullScreenQuad::FullScreenQuad(std::string _vsLocation, std::string _psLocation, DXCore* _driver)
{
	// get shaders
	std::string vertexS = GetFileData(_vsLocation);
	std::string pixelS = GetFileData(_psLocation);

	// compile shaders
	CompileVertexShader(vertexS, *_driver);
	CompilePixelShader(pixelS, *_driver);
}

void FullScreenQuad::CompileVertexShader(std::string _shader, DXCore& _driver)
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
}

void FullScreenQuad::CompilePixelShader(std::string _shader, DXCore& _driver)
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

void FullScreenQuad::UpdateTexture(std::string _name, ID3D11ShaderResourceView* srv, DXCore* _driver)
{
	_driver->devicecontext->PSSetShaderResources(textureBindPointsPS[_name], 1, &srv);
}

void FullScreenQuad::Apply(DXCore* _driver)
{
	_driver->devicecontext->IASetInputLayout(NULL); // set layout

	_driver->devicecontext->VSSetShader(vertexShader, NULL, 0); // apply vertex shader
	_driver->devicecontext->PSSetShader(pixelShader, NULL, 0); // apply pixel shader
}

void FullScreenQuad::Draw(DXCore* _driver)
{
	Apply(_driver);
	_driver->devicecontext->Draw(3, 0);
}

FullScreenQuad::~FullScreenQuad()
{
	vertexShader->Release();
	pixelShader->Release();
}
