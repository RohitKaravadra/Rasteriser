#pragma once

#include <iostream>
#include "Driver.h"
#include "Utilities.h"
#include <map>

class Sampler
{
	ID3D11SamplerState* state;
public:
	Sampler(DXCore& _driver);
	void Bind(DXCore& _driver) const;
};

class Texture
{
public:
	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* srv;

	bool Load(std::string _location, DXCore& _driver);
	void Init(int _width, int _height, int _channels, DXGI_FORMAT _format, unsigned char* _data, DXCore& _driver);
	void Free();
	~Texture();
};

static class TextureManager
{
	static std::map<std::string, Texture*> textures;
	static DXCore* driver;
	TextureManager() = default;
public:
	static void Init(DXCore* _driver);
	static void load(std::string _location);
	static ID3D11ShaderResourceView* find(std::string name);
	static void unload(std::string name);
	static void Free();
};

