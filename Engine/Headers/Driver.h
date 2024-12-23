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

// DirectX driver class
class DXCore
{
public:
	ID3D11Device* device; // graphic device
	ID3D11DeviceContext* devContext;
	IDXGISwapChain* swapchain;

	ID3D11RenderTargetView* backbufferRenderTargetView;
	ID3D11Texture2D* backbuffer;

	ID3D11DepthStencilView* depthStencilView;
	ID3D11Texture2D* depthbuffer;

	D3D11_VIEWPORT viewport;

	ID3D11RasterizerState* rasterizerState;

	void Init(int _width, int _height, const HWND& _hwnd, bool _fullScreen);
	void SetRasterizerState(DrawType _type = DrawType::Solid);
	// clear backbuffer
	void ClearBackbuffer();
	// present swapchain
	void Present();
	~DXCore();
};

