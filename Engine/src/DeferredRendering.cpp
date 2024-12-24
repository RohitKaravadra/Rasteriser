#include "../hdr/DeferredRendering.h"

RenderTarget::RenderTarget(unsigned int _width, unsigned int _height, DXGI_FORMAT _format, DXCore* _driver)
{
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
	texDesc.Width              = _width;
	texDesc.Height             = _height;
	texDesc.MipLevels          = 1;
	texDesc.ArraySize          = 1;
	texDesc.Format             = _format;
	texDesc.SampleDesc.Count   = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage              = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags          = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags     = 0;
	texDesc.MiscFlags          = 0;

	// create render target view description
	D3D11_RENDER_TARGET_VIEW_DESC viewDesc;
	ZeroMemory(&viewDesc, sizeof(viewDesc));
	viewDesc.Format             = texDesc.Format;
	viewDesc.ViewDimension      = D3D11_RTV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipSlice = 0;

	// create shader resource view description
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format                    = texDesc.Format;
	srvDesc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels       = 1;

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
	texDesc.Width              = _width;
	texDesc.Height             = _height;
	texDesc.MipLevels          = 1;
	texDesc.ArraySize          = 1;
	texDesc.Format             = DXGI_FORMAT_R32_TYPELESS;
	texDesc.SampleDesc.Count   = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage              = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags     = 0;
	texDesc.MiscFlags          = 0;

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format             = DXGI_FORMAT_D32_FLOAT;  // Depth format for DSV
	dsvDesc.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	// create shader resource view description
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format                    = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels       = 1;

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
	vertexShader = new VertexShader("FullQuad", _vsLocation, _driver, LayoutType::None);
	pixelShader  = new PixelShader("FullQuad", _psLocation, _driver);
}

void FullScreenQuad::Apply(DXCore* _driver)
{
	vertexShader->Apply(_driver);
	pixelShader ->Apply(_driver);
}

void FullScreenQuad::Draw(DXCore* _driver)
{
	Apply(_driver);
	_driver->devContext->Draw(3, 0);
}

void FullScreenQuad::UpdateConstant(std::string _bufferName, std::string _varName, void* _data)
{
	pixelShader->UpdateConstant(_bufferName, _varName, _data);
}

FullScreenQuad::~FullScreenQuad()
{
	delete vertexShader, pixelShader;
}

GBuffer::GBuffer(unsigned int _width, unsigned int _height, DXCore* _driver)
{
	albedo  = new RenderTarget(_width, _height, DXGI_FORMAT_R16G16B16A16_FLOAT, _driver);
	normal  = new RenderTarget(_width, _height, DXGI_FORMAT_R16G16B16A16_FLOAT, _driver);
	zBuffer = new ZBuffer(_width, _height, _driver);
}

void GBuffer::Clear(DXCore* _driver)
{
	float color[4] = { 0,0,0,1 };
	_driver->devContext->ClearRenderTargetView(albedo->view, color);
	_driver->devContext->ClearRenderTargetView(normal->view, color);

	_driver->devContext->ClearDepthStencilView(zBuffer->depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

GBuffer::~GBuffer()
{
	delete albedo, normal, zBuffer;
}

void DeferredRenderer::Init(unsigned int _width, unsigned int _height, DXCore* _driver)
{
	driver         = _driver;
	gBuffer        = new GBuffer(_width, _height, _driver);
	lightMap       = new ZBuffer(_width, _height, _driver);
	depthOnlyPixel = new PixelShader("DepthOnly", "Resources/Shaders/Pixel/DepthOnlyPixel.hlsl", _driver);
	fullScreenQuad = new FullScreenQuad("Resources/Shaders/Vertex/FullScreenQuadVertex.hlsl",
		"Resources/Shaders/Pixel/FullScreenQuadPixel.hlsl", _driver);

	// set render target views buffer
	renderTargetViews[0] = gBuffer->albedo->view;
	renderTargetViews[1] = gBuffer->normal->view;

	// set shader resource view buffer
	shaderResourceViews[0] = gBuffer->albedo->srv;
	shaderResourceViews[1] = gBuffer->normal->srv;
	shaderResourceViews[2] = gBuffer->zBuffer->srv;
	shaderResourceViews[3] = lightMap->srv;
}

void DeferredRenderer::Clear()
{
	// clear rendertextures and zbuffer
	gBuffer->Clear(driver);
	// clear lightmap
	driver->devContext->ClearDepthStencilView(lightMap->depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	// clear backbuffer
	driver->ClearBackbuffer();
}

void DeferredRenderer::SetGeometryPass()
{
	// set albedo and normal render texture views for geometry pass
	driver->devContext->OMSetRenderTargets(2, renderTargetViews, gBuffer->zBuffer->depthStencilView);
}

void DeferredRenderer::SetLightPass()
{
	// set depth only pass to generate lightmap
	driver->devContext->OMSetRenderTargets(0, nullptr, lightMap->depthStencilView);
	// apply depth only pixel for depth only pass (no texture lookups)
	depthOnlyPixel->Apply(driver);
}

void DeferredRenderer::UpdateConstant(std::string _bufferName, std::string _varName, void* _data)
{
	// update fullscreenquad pixel shader constanbuffer
	fullScreenQuad->UpdateConstant(_bufferName, _varName, _data);
}

void DeferredRenderer::Draw()
{
	// reset backbuffer for final draw
	driver->devContext->OMSetRenderTargets(1, &driver->backbufferRenderTargetView, nullptr);
	// set resources in fullscreenquad shader
	driver->devContext->PSSetShaderResources(0, 4, shaderResourceViews);
	// draw fullscreen quad to the screen
	fullScreenQuad->Draw(driver);
}

DeferredRenderer::~DeferredRenderer()
{
	delete gBuffer, fullScreenQuad;
	delete lightMap, depthOnlyPixel;
}