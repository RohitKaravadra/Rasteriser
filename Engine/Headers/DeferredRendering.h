#pragma once
#include "Shader.h"
#include "GMath.h"

// render target class
class RenderTarget
{
public:
	ID3D11Texture2D* texture;
	ID3D11RenderTargetView* view;
	ID3D11ShaderResourceView* srv; // shader resource view
	RenderTarget(unsigned int _width, unsigned int _height, DXGI_FORMAT _format, DXCore* _driver);
	~RenderTarget();
};

class ZBuffer
{
public:
	ID3D11Texture2D* texture;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11ShaderResourceView* srv;
	ZBuffer(unsigned int _width, unsigned int _height, DXCore* _driver);
	~ZBuffer();
};

// Full screen quad shader for Deffered shading
class FullScreenQuad
{
	VertexShader* vertexShader;
	PixelShader* pixelShader;

	void Apply(DXCore* _driver);
public:
	FullScreenQuad(std::string _vsLocation, std::string _psLocation, DXCore* _driver);
	// draw texture to full screen quad and to screen
	void Draw(DXCore* _driver);
	// update shader constant buffer
	void UpdateConstant(std::string _bufferName, std::string _varName, void* _data);
	~FullScreenQuad();
};

struct GBuffer
{
	RenderTarget* albedo;
	RenderTarget* normal;
	ZBuffer* zBuffer;

	GBuffer(unsigned int _width, unsigned int _height, DXCore* _driver);
	void Clear(DXCore* _driver);
	~GBuffer();
};

class DeferredRenderer
{
	GBuffer* gBuffer;
	ZBuffer* lightMap;

	PixelShader* depthOnlyPixel;

	ID3D11RenderTargetView* renderTargetViews[2];
	ID3D11ShaderResourceView* shaderResourceViews[4];

	DXCore* driver;
	FullScreenQuad* fullScreenQuad;
public:
	void Init(unsigned int _width, unsigned int _height, DXCore* _driver);
	void SetGeometryPass();
	void SetLightPass();
	// update shader constant buffer
	void UpdateConstant(std::string constantBufferName, std::string variableName, void* data);
	void clear();
	void Draw();
	~DeferredRenderer();
};