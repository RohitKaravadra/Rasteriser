#pragma once

#include <D3D11.h>
#include <dxgi1_6.h>
#include<d3dcompiler.h>
#include <d3d11shader.h>

#include <vector>
#include <iostream>
#include<fstream>
#include<sstream>

#pragma comment(lib, "D3D11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "DXGI.lib")

#pragma comment(lib, "dxguid.lib")

class Adapter
{
public:
	IDXGIAdapter1* adapter; // best available adapter
	// finds and stores best available graphics adapter
	void FindAdapter();
};

enum DrawType
{
	Solid,
	Outline
};

// render target class
class RenderTarget
{
public:
	ID3D11Texture2D* texture;
	ID3D11RenderTargetView* view;
	ID3D11ShaderResourceView* srv; // shader resource view
	~RenderTarget();
};

// DirectX driver class
class DXCore
{
public:
	ID3D11Device* device; // graphic device
	ID3D11DeviceContext* devicecontext;
	IDXGISwapChain* swapchain;

	ID3D11RenderTargetView* backbufferRenderTargetView;
	ID3D11Texture2D* backbuffer;

	ID3D11DepthStencilView* depthStencilView;
	ID3D11Texture2D* depthbuffer;

	D3D11_VIEWPORT viewport;

	ID3D11RasterizerState* rasterizerState;

	void Init(int _width, int _height, const HWND& _hwnd, bool _fullScreen);
	void UpdateRasterizerState(DrawType _type = DrawType::Solid);

	void CreateRenderTarget(unsigned int _width, unsigned int _height, DXGI_FORMAT _format, RenderTarget* _renderTarget);
	// clear backbuffer
	void ClearBackbuffer();
	// apply backbuffer
	void ApplyBackbuffer();
	// clear render target
	void ClearRenderTarget(RenderTarget* _renderTarget);
	// apply render target
	void ApplyRenderTarget(RenderTarget* _renderTarget);
	// apply multiple render targets
	void ApplyRenderTargets(unsigned int _total, ID3D11RenderTargetView** _buffer);
	// present swapchain
	void Present();
	~DXCore();
};

