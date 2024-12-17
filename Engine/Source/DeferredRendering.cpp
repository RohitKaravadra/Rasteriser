#include "DeferredRendering.h"

RenderTarget::RenderTarget(unsigned int _width, unsigned int _height, DXGI_FORMAT _format, DXCore* _driver)
{
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
	texDesc.Width = _width;
	texDesc.Height = _height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = _format;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	// create render target view description
	D3D11_RENDER_TARGET_VIEW_DESC viewDesc;
	ZeroMemory(&viewDesc, sizeof(viewDesc));
	viewDesc.Format = texDesc.Format;
	viewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipSlice = 0;

	// create shader resource view description
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	if (SUCCEEDED(_driver->device->CreateTexture2D(&texDesc, NULL, &texture)))
		if (SUCCEEDED(_driver->device->CreateRenderTargetView(texture, &viewDesc, &view)))
			_driver->device->CreateShaderResourceView(texture, &srvDesc, &srv);
}

RenderTarget::~RenderTarget()
{
	texture->Release();
	view->Release();
	srv->Release();
}

ZBuffer::ZBuffer(unsigned int _width, unsigned int _height, DXCore* _driver)
{
	D3D11_TEXTURE2D_DESC texDesc{};
	texDesc.Width = _width;
	texDesc.Height = _height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;  // Depth format for DSV
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	// create shader resource view description
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	if (SUCCEEDED(_driver->device->CreateTexture2D(&texDesc, NULL, &texture)))
	{
		_driver->device->CreateDepthStencilView(texture, &dsvDesc, &depthStencilView);
		_driver->device->CreateShaderResourceView(texture, &srvDesc, &srv);
	}
}

ZBuffer::~ZBuffer()
{
	texture->Release();
	depthStencilView->Release();
	srv->Release();
}

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
	_driver->devicecontext->PSSetShaderResources(textureBindPointsPS[_name], 1, &srv);
}

FullScreenQuad::~FullScreenQuad()
{
	vertexShader->Release();
	pixelShader->Release();
}

GBuffer::GBuffer(unsigned int _width, unsigned int _height, DXCore* _driver)
{
	albedo = new RenderTarget(_width, _height, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, _driver);
	normal = new RenderTarget(_width, _height, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, _driver);
	tangent = new RenderTarget(_width, _height, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, _driver);
	zBuffer = new ZBuffer(_width, _height, _driver);

	renderTargetViews[0] = albedo->view;
	renderTargetViews[1] = normal->view;
	renderTargetViews[2] = tangent->view;

	shaderResourceViews[0] = albedo->srv;
	shaderResourceViews[1] = normal->srv;
	shaderResourceViews[2] = tangent->srv;
	shaderResourceViews[3] = zBuffer->srv;

	fullScreenQuad = new FullScreenQuad("Resources/Shaders/Vertex/FullScreenQuadVertex.hlsl",
		"Resources/Shaders/Pixel/FullScreenQuadPixel.hlsl", _driver);
}

void GBuffer::Clear(DXCore* _driver)
{
	float color[4] = { 0,0,0,1 };
	_driver->devicecontext->ClearRenderTargetView(albedo->view, color);
	_driver->devicecontext->ClearRenderTargetView(normal->view, color);
	_driver->devicecontext->ClearRenderTargetView(tangent->view, color);

	_driver->devicecontext->ClearDepthStencilView(zBuffer->depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	_driver->ClearBackbuffer();
}

void GBuffer::Apply(DXCore* _driver)
{
	_driver->devicecontext->OMSetRenderTargets(3, renderTargetViews, zBuffer->depthStencilView);
}

void GBuffer::Draw(DXCore* _driver)
{
	_driver->devicecontext->OMSetRenderTargets(1, &_driver->backbufferRenderTargetView, _driver->depthStencilView);
	// set all textures (texture buffer)
	_driver->devicecontext->PSSetShaderResources(0, 4, shaderResourceViews);
	fullScreenQuad->Draw(_driver);
}

GBuffer::~GBuffer()
{
	delete albedo, normal, tangent, fullScreenQuad, zBuffer;
}

void DeferredRenderer::Init(unsigned int _width, unsigned int _height, DXCore* _driver)
{
	driver = _driver;

	gBuffer = new GBuffer(_width, _height, _driver);
}

void DeferredRenderer::SetPassOne()
{
	gBuffer->Clear(driver);
	driver->ClearBackbuffer();
	gBuffer->Apply(driver);
}

void DeferredRenderer::SetPassTwo()
{
}

void DeferredRenderer::Draw()
{
	gBuffer->Draw(driver);
	driver->Present();
}

DeferredRenderer::~DeferredRenderer()
{
	delete gBuffer;
}