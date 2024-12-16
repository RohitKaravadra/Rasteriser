#include "Shader.h"
#include "Utilities.h"

FullScreenQuad::FullScreenQuad(std::string _vsLocation, std::string _psLocation, DXCore* _driver)
{
	// compile shaders
	CompileVertexShader(_vsLocation, *_driver);
	CompilePixelShader(_psLocation, *_driver);
}

void FullScreenQuad::CompileVertexShader(std::string _location, DXCore& _driver)
{
	ID3DBlob* compiledShader = Shader::GetCompiled(ShaderStage::VertexShader, _location); // store compiled vertex shader

	// create and store vertex shader
	_driver.device->CreateVertexShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), NULL, &vertexShader);

	compiledShader->Release();
}

void FullScreenQuad::CompilePixelShader(std::string _location, DXCore& _driver)
{
	ID3DBlob* compiledShader = Shader::GetCompiled(ShaderStage::PixelShader, _location); // store compiled pixel shader

	// create and store pixel shader
	_driver.device->CreatePixelShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), NULL, &pixelShader);

	// create constant buffer
	std::cout << "Quad" << std::endl;
	ConstantBufferReflection reflection;
	reflection.build(_driver, compiledShader, psConstantBuffers, textureBindPointsPS, ShaderStage::PixelShader);

	compiledShader->Release();
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

void FullScreenQuad::SetTexture(std::string _name, ID3D11ShaderResourceView* srv, DXCore* _driver)
{
	//std::cout << _name << " - " << textureBindPointsPS[_name] << std::endl;
	_driver->devicecontext->PSSetShaderResources(textureBindPointsPS[_name], 1, &srv);
}

FullScreenQuad::~FullScreenQuad()
{
	vertexShader->Release();
	pixelShader->Release();
}
