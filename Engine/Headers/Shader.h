#pragma once
#include "Driver.h"
#include <map>

enum ShaderStage
{
	VertexShader,
	PixelShader
};

enum ShaderType
{
	Normal,
	Animated,
	Instancing
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

	std::vector<ConstantBuffer> psConstantBuffers;
	std::vector<ConstantBuffer> vsConstantBuffers;

	std::map<std::string, int> textureBindPointsVS;
	std::map<std::string, int> textureBindPointsPS;

	// build layeout for vertex input
	void BuildLayout(ShaderType _type, ID3DBlob* compiledVertexShader, DXCore& _driver);
	// compile vertex shader and bind layout
	void CompileVertexShader(std::string _shader, ShaderType _type, DXCore& _driver);
	// compile pixel shader
	void CompilePixelShader(std::string _shader, DXCore& _driver);
	// update the value inside the constant buffer
	void UpdateConstant(std::string constantBufferName, std::string variableName, void* data, std::vector<ConstantBuffer>& buffers);

public:
	// create and compile shader
	Shader(std::string _name, std::string _vsLocation, std::string _psLocation, DXCore& _driver, ShaderType _type = ShaderType::Normal);
	// apply shader
	void Apply(DXCore& _driver);
	// update shader constant constant buffer
	void UpdateConstant(ShaderStage _type, std::string constantBufferName, std::string variableName, void* data);
	// update texture
	void UpdateTexture(ShaderStage _type, std::string _name, ID3D11ShaderResourceView* srv, DXCore& _driver);
	// destroy shader
	~Shader();
};

// preloades shaders and stores it
static class ShaderManager
{
	static std::map<std::string, Shader*> shaders; // store shaders
	static DXCore* driver; // reference to the device
	static std::string current; // current applied shader

	ShaderManager() = default;
public:
	static void Init(DXCore* _driver);
	// add shader to list
	static void Add(std::string _name, std::string _vsLocation, std::string _psLocation, ShaderType _type = ShaderType::Normal);
	// set shader
	static void Set(std::string _name);
	// apply shader of given name
	static void Apply();
	// update constant of a shader with given name
	static void UpdateConstant(ShaderStage _type, std::string constantBufferName,
		std::string variableName, void* data);
	// update shader constant by shader name
	static void UpdateConstant(std::string _name, ShaderStage _type, std::string constantBufferName,
		std::string variableName, void* data);
	// update constant of a shader with given name
	static void UpdateConstantForAll(ShaderStage _type, std::string constantBufferName,
		std::string variableName, void* data);
	// update texture
	static void UpdateTexture(ShaderStage _type, std::string _name, ID3D11ShaderResourceView* srv);
	// update texture of perticular shader
	static void UpdateTexture(std::string _shader, ShaderStage _type, std::string _name, ID3D11ShaderResourceView* srv);
	// destroy all shaders
	static void Free();
};
