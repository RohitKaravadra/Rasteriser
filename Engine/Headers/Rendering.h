#pragma once
#include "Shader.h"

class GBuffer
{
	RenderTarget* albedo;
	RenderTarget* normal;
	RenderTarget* tangent;

	ID3D11RenderTargetView* renderTargetViews[3];

	DXCore* driver;
public:

	FullScreenQuad* fullScreenQuad;

	void Init(unsigned int _width, unsigned int _height, DXCore* _driver)
	{
		driver = _driver;

		albedo = new RenderTarget();
		normal = new RenderTarget();
		tangent = new RenderTarget();

		driver->CreateRenderTarget(_width, _height, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, albedo);
		driver->CreateRenderTarget(_width, _height, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, normal);
		driver->CreateRenderTarget(_width, _height, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, tangent);

		renderTargetViews[0] = albedo->view;
		renderTargetViews[1] = normal->view;
		renderTargetViews[1] = tangent->view;

		fullScreenQuad = new FullScreenQuad("Resources/Shaders/Vertex/FullScreenQuadVertex.hlsl", "Resources/Shaders/Pixel/FullScreenQuadPixel.hlsl", driver);
	}

	void Clear()
	{
		driver->ClearBackbuffer();
		driver->ClearRenderTarget(albedo);
		driver->ClearRenderTarget(normal);
		driver->ClearRenderTarget(tangent);
	}

	void Apply()
	{
		driver->ApplyRenderTargets(2, renderTargetViews);
	}

	void Draw()
	{
		driver->ApplyBackbuffer();

		fullScreenQuad->SetTexture("alb", albedo->srv, driver);
		fullScreenQuad->SetTexture("nor", normal->srv, driver);
		fullScreenQuad->SetTexture("tan", tangent->srv, driver);
		fullScreenQuad->Draw(driver);

		driver->Present();
	}

	~GBuffer()
	{
		delete albedo, normal, tangent, fullScreenQuad;
	}
};