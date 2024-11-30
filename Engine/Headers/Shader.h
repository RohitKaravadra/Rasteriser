#pragma once
#include "Driver.h"
#include <map>

// read data from a file and return data in string format
static std::string GetFileData(std::string _fileName);

enum ShaderStage
{
	VertexShader,
	PixelShader
};

struct ConstantBufferVariable
{
	unsigned int offset;
	unsigned int size;
};

class ConstantBuffer
{
public:
	std::string name;
	std::map<std::string, ConstantBufferVariable> constantBufferData;

	ID3D11Buffer* cb;
	unsigned char* buffer;
	unsigned int cbSizeInBytes;

	int dirty;
	int index;

	ShaderStage shaderStage;

	void init(DXCore& _driver, unsigned int sizeInBytes,
		int constantBufferIndex, ShaderStage _shaderStage);
	void update(std::string name, void* data);
	void upload(DXCore& _driver);
	void free();
};

class ConstantBufferReflection
{
public:
	void build(DXCore& _driver, ID3DBlob* shader, std::vector<ConstantBuffer>& buffers,
		std::map<std::string, int>& textureBindPoints, ShaderStage shaderStage);
};

// Shader class to handle creation and compilation of shaders
class Shader
{
	std::string name;

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* layout; // layout for vertex shader

	//ID3D11Buffer* constantBuffer;
	std::vector<ConstantBuffer> psConstantBuffers;
	std::vector<ConstantBuffer> vsConstantBuffers;
	std::map<std::string, int> textureBindPointsVS;
	std::map<std::string, int> textureBindPointsPS;

	// compile vertex shader and bind layout
	void CompileVertexShader(std::string _shader, bool _animated, DXCore& _driver);
	// compile pixel shader
	void CompilePixelShader(std::string _shader, DXCore& _driver);
	// update the value inside the constant buffer
	void UpdateConstant(std::string constantBufferName, std::string variableName, void* data, std::vector<ConstantBuffer>& buffers);

public:
	// create and compile shader
	void Init(std::string _vsLocation, std::string _psLocation, DXCore& _driver, bool _animated = false);
	// apply shader
	void Apply(DXCore& _driver);
	// update shader constant constant buffer
	void UpdateConstant(ShaderStage _type, std::string constantBufferName, std::string variableName, void* data);
};

// preloades shaders and stores it
static class ShaderManager
{
	static std::map<std::string, Shader> shaders; // store shaders
	static DXCore* driver; // reference to the device
	static std::string current; // current applied shader

	ShaderManager() = default;
public:
	static void Init(DXCore& _driver);
	// add shader to list
	static void Add(std::string _name, std::string _vsLocation, std::string _psLocation, bool _animated = false);
	// set shader
	static void Set(std::string _name);
	// apply shader of given name
	static void Apply();
	// update constant of a shader with given name
	static void UpdateConstant(ShaderStage _type, std::string constantBufferName, std::string variableName, void* data);
	// update constant of a shader with given name
	static void UpdateConstantForAll(ShaderStage _type, std::string constantBufferName, std::string variableName, void* data);
};
