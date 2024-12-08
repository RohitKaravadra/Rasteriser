#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Texture.h"

Sampler::Sampler(DXCore& _driver)
{
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	_driver.device->CreateSamplerState(&samplerDesc, &state);
}

void Sampler::Bind(DXCore& _driver) const
{
	_driver.devicecontext->PSSetSamplers(0, 1, &state);
}

bool Texture::Load(std::string _location, DXCore& _driver)
{
	int width = 0;
	int height = 0;
	int channels = 0;
	unsigned char* texels = stbi_load(_location.c_str(), &width, &height, &channels, 0);

	if (width == 0 || height == 0)
		return false;

	if (channels == 3) {
		channels = 4;
		unsigned char* texelsWithAlpha = new unsigned char[width * height * channels];
		for (int i = 0; i < (width * height); i++) {
			texelsWithAlpha[i * 4] = texels[i * 3];
			texelsWithAlpha[(i * 4) + 1] = texels[(i * 3) + 1];
			texelsWithAlpha[(i * 4) + 2] = texels[(i * 3) + 2];
			texelsWithAlpha[(i * 4) + 3] = 255;
		}
		Init(width, height, channels, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, texelsWithAlpha, _driver);
		delete[] texelsWithAlpha;
	}
	else {
		Init(width, height, channels, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, texels, _driver);
	}
	stbi_image_free(texels);

	return true;
}

void Texture::Free()
{
	if (srv != nullptr)
		srv->Release();
	if (texture != nullptr)
		texture->Release();
}

Texture::~Texture()
{
	Free();
}

void Texture::Init(int _width, int _height, int _channels, DXGI_FORMAT _format, unsigned char* _data, DXCore& _driver)
{
	D3D11_TEXTURE2D_DESC texDesc;
	memset(&texDesc, 0, sizeof(D3D11_TEXTURE2D_DESC));
	texDesc.Width = _width;
	texDesc.Height = _height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = _format;
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	memset(&initData, 0, sizeof(D3D11_SUBRESOURCE_DATA));
	initData.pSysMem = _data;
	initData.SysMemPitch = _width * _channels;
	_driver.device->CreateTexture2D(&texDesc, &initData, &texture);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = _format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	_driver.device->CreateShaderResourceView(texture, &srvDesc, &srv);
}

std::map<std::string, Texture*> TextureManager::textures;
DXCore* TextureManager::driver = nullptr;

void TextureManager::Init(DXCore* _driver)
{
	driver = _driver;
}

void TextureManager::load(std::string _location)
{
	std::string name = ExtractName(_location);
	if (driver == nullptr || textures.find(name) != textures.cend())
		return;

	Texture* texture = new Texture();
	// check if tetxure is loaded or not
	if (texture->Load(_location, *driver))
		textures.insert({ name, texture });
	else
	{
		std::string msg = "Texture Not Loaded : " + name;
		MessageBoxA(NULL, (char*)msg.c_str(), "Vertex Shader Error", 0);
	}
}

ID3D11ShaderResourceView* TextureManager::find(std::string _name)
{
	// check if texture is available or not 
	if (textures.find(_name) == textures.cend())
		return textures.begin()->second->srv;
	return textures[_name]->srv;
}

void TextureManager::unload(std::string _name)
{
	if (textures.find(_name) == textures.end())
		return;

	textures[_name]->Free();
	textures.erase(_name);
}
void TextureManager::Free()
{
	for (auto it = textures.cbegin(); it != textures.cend(); )
	{
		it->second->Free();
		textures.erase(it++);
	}
}
