#include "Driver.h"
#include <iostream>

// enables detection of Nvidia adapters
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

#pragma region Adapter

void Adapter::FindAdapter()
{
	// find all available adapters
	IDXGIAdapter1* adapterf;
	std::vector<IDXGIAdapter1*> adapters; // all avaialable adapters
	IDXGIFactory6* factory = NULL;
	CreateDXGIFactory(__uuidof(IDXGIFactory6), (void**)&factory);
	int i = 0;
	while (factory->EnumAdapters1(i, &adapterf) != DXGI_ERROR_NOT_FOUND)
	{
		adapters.push_back(adapterf);
		i++;
	}

	// select best adapater according to video memory
	long long maxVideoMemory = 0;
	int useAdapterIndex = 0;
	for (int i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC desc;
		adapters[i]->GetDesc(&desc);
		if (desc.DedicatedVideoMemory > maxVideoMemory)
		{
			maxVideoMemory = desc.DedicatedVideoMemory;
			useAdapterIndex = i;
		}
	}

	// assign best adapter
	adapter = adapters[useAdapterIndex];
	factory->Release(); // release factory data
}

#pragma endregion

#pragma region Device

void DXCore::Init(int _width, int _height, const HWND& _hwnd, bool _fullScreen)
{
	// setting swapchain data
	DXGI_SWAP_CHAIN_DESC sd;
	memset(&sd, 0, sizeof(DXGI_SWAP_CHAIN_DESC));

	sd.BufferCount = 1;
	sd.BufferDesc.Width = _width;
	sd.BufferDesc.Height = _height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 120;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = _hwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = _fullScreen ? false : true;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// setting version of DirectX (11 for this project)
	D3D_FEATURE_LEVEL fl;
	fl = D3D_FEATURE_LEVEL_11_0;

	// find best adapter
	Adapter adptr;
	adptr.FindAdapter();

	// create and initialize device, device context and swapchain
	D3D11CreateDeviceAndSwapChain(adptr.adapter,
		D3D_DRIVER_TYPE_UNKNOWN,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		&fl,
		1,
		D3D11_SDK_VERSION,
		&sd,
		&swapchain,
		&device,
		NULL,
		&devicecontext);

	swapchain->SetFullscreenState(_fullScreen, NULL); // set fullscreen mode
	if (swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backbuffer) >= 0) // get back buffer from swapchain
		device->CreateRenderTargetView(backbuffer, NULL, &backbufferRenderTargetView); // create render target view from back buffer

	// initialize depth buffer data
	D3D11_TEXTURE2D_DESC dsvDesc;
	dsvDesc.Width = _width;
	dsvDesc.Height = _height;
	dsvDesc.MipLevels = 1;
	dsvDesc.ArraySize = 1;
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.SampleDesc.Count = 1;
	dsvDesc.SampleDesc.Quality = 0;
	dsvDesc.Usage = D3D11_USAGE_DEFAULT;
	dsvDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dsvDesc.CPUAccessFlags = 0;
	dsvDesc.MiscFlags = 0;

	if (device->CreateTexture2D(&dsvDesc, NULL, &depthbuffer) >= 0) // create depth buffer (Texture)
		device->CreateDepthStencilView(depthbuffer, NULL, &depthStencilView); // create depth stencil view from depth buffer

	// set render target view and back buffer view to device context
	devicecontext->OMSetRenderTargets(1, &backbufferRenderTargetView, depthStencilView);

	// create viewport
	viewport.Width = (float)_width;
	viewport.Height = (float)_height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	devicecontext->RSSetViewports(1, &viewport); // set view port (1 for 0th index)

	// create rasterizer state for draw call
	D3D11_RASTERIZER_DESC rsdesc;
	ZeroMemory(&rsdesc, sizeof(D3D11_RASTERIZER_DESC));
	rsdesc.FillMode = D3D11_FILL_SOLID;
	rsdesc.CullMode = D3D11_CULL_NONE;
	device->CreateRasterizerState(&rsdesc, &rasterizerState);

	// set rasterizer state for draw call
	UpdateRasterizerState();
}

void DXCore::UpdateRasterizerState(DrawType _type)
{
	// create rasterizer state for draw call
	D3D11_RASTERIZER_DESC rsdesc;
	ZeroMemory(&rsdesc, sizeof(D3D11_RASTERIZER_DESC));
	rsdesc.FillMode = _type == DrawType::Solid ? D3D11_FILL_SOLID : D3D11_FILL_WIREFRAME;
	rsdesc.CullMode = D3D11_CULL_NONE;
	device->CreateRasterizerState(&rsdesc, &rasterizerState);

	// set rasterizer state for draw call
	devicecontext->RSSetState(rasterizerState);
}

void DXCore::CreateRenderTarget(unsigned int _width, unsigned int _height,
	ID3D11Texture2D* _texture, ID3D11RenderTargetView* _view,
	ID3D11ShaderResourceView* _srv)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = _width;
	textureDesc.Height = _height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	D3D11_RENDER_TARGET_VIEW_DESC viewDesc;
	ZeroMemory(&viewDesc, sizeof(viewDesc));
	viewDesc.Format = textureDesc.Format;
	viewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipSlice = 0;

	if (device->CreateTexture2D(&textureDesc, NULL, &_texture) >= 0)
		device->CreateRenderTargetView(_texture, &viewDesc, &_view);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(_texture, &srvDesc, &_srv);
}

void DXCore::Clear()
{
	float ClearColour[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	// clear back buffer
	devicecontext->ClearRenderTargetView(backbufferRenderTargetView, ClearColour);
	// clear depth and stencil
	devicecontext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DXCore::ApplyBackbufferView()
{
	devicecontext->OMSetRenderTargets(1, &backbufferRenderTargetView, depthStencilView);
}

void DXCore::Present() { swapchain->Present(0, 0); }

DXCore::~DXCore()
{
	// release all device data
	backbuffer->Release();
	depthStencilView->Release();
	depthbuffer->Release();
	backbufferRenderTargetView->Release();
	swapchain->Release();
	devicecontext->Release();
	device->Release();
}

#pragma endregion

