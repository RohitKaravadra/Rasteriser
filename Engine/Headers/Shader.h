#pragma once
#include "Drivers.h"
#include <map>
#include "GMath.h"

// read data from a file and return data in string format
static std::string GetFileData(std::string fileName);

// constant buffer for shader (for testing not finalize)
struct alignas(16) ConstantBuffer
{
	float time;
};

struct alignas(16) ConstantBuffer2
{
	float time;
	float padding[3];
	Vec4 lights[4];
};

// Shader class to handle creation and compilation of shaders
class Shader
{
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* layout; // layout for vertex shader

	ID3D11Buffer* constantBuffer;

public:
	// create and compile shader
	void Init(std::string _VSLocation, std::string _PSLocation, DXCore& _driver);
	// compile vertex shader and bind layout
	void CompileVertexShader(std::string _shader, DXCore& _driver);
	// compile pixel shader
	void CompilePixelShader(std::string _shader, DXCore& _driver);
	// apply shader
	void ApplyShader(DXCore& _driver);
	// initialize constant buffer
	void InitConstBuffer(unsigned int, DXCore&);
	// set constant buffer
	void UpdateConstBuffer(void*, unsigned int, DXCore&);
};

// preloades shaders and stores it
static class ShaderManager
{
	static std::map<std::string, Shader> shaders;
public:
	static void Load(std::vector<std::string>& _shaders, DXCore& _driver);
	static bool GetShader(std::string _name, Shader& _shader);
};
