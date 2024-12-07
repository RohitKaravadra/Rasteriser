#pragma once
#include "Driver.h"

class RenderTarget
{
public:
	ID3D11Texture2D* texture;
	ID3D11RenderTargetView* view;
	ID3D11ShaderResourceView* srv;

	RenderTarget(unsigned int _width, unsigned int _height, DXCore* _driver)
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

		if (_driver->device->CreateTexture2D(&textureDesc, NULL, &texture) >= 0)
			_driver->device->CreateRenderTargetView(texture, &viewDesc, &view);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;

		_driver->device->CreateShaderResourceView(texture, &srvDesc, &srv);
	}

	void Apply(DXCore* _driver)
	{
		_driver->devicecontext->OMSetRenderTargets(1, &view, _driver->depthStencilView);
	}

	void Clear(DXCore* _driver)
	{
		float ClearColour[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		_driver->devicecontext->ClearRenderTargetView(view, ClearColour);
	}

	~RenderTarget()
	{
		texture->Release();
		view->Release();
		srv->Release();
	}
};
