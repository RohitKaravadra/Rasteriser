#pragma once

#include <iostream>
#include "Driver.h"
#include <map>

class Sampler
{
public:
	ID3D11SamplerState* state;
	void Init(DXCore& _driver);
	void Bind(DXCore& _driver);
};

class Texture
{
public:
	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* srv;

	void Load(std::string _location, DXCore& _driver);
	void Init(int _width, int _height, int _channels, DXGI_FORMAT _format, unsigned char* _data, DXCore& _driver);
	void Free();
	~Texture();
};

class TextureManager
{
public:
	std::map<std::string, Texture*> textures;

	void load(std::string _location, DXCore& core);
	ID3D11ShaderResourceView* find(std::string name);
	void unload(std::string name);
	~TextureManager();
};

