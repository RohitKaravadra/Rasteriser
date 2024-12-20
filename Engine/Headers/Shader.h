#pragma once
#include "Driver.h"
#include <map>

enum ShaderStage
{
	Vertex,
	Pixel
};

enum LayoutType
{
	Normal,
	Animated,
	Instancing,
	None
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

// get compiled shader
ID3DBlob* GetCompiled(ShaderStage _stage, std::string _location, bool _load = false);

// vertex shader class for vertex shader data and operations
class VertexShader
{
	std::string name;

	ID3D11VertexShader* shader;
	ID3D11InputLayout* layout;

	// shader reflection data
	std::vector<ConstantBuffer> constBuffer;
	std::map<std::string, int> texBindPoints;

	void BuildLayout(LayoutType _type, ID3DBlob* _shader, DXCore* _driver);
	// compile vertex shader and bind layout
	void CompileVertexShader(std::string _shader, LayoutType _type, DXCore* _driver);
public:
	VertexShader(std::string _name, std::string _location, DXCore* _driver, LayoutType _type = LayoutType::Normal);
	// update constant buffer variable
	void UpdateConstant(std::string _bufferName, std::string _varName, void* _data);
	// apply shader
	void Apply(DXCore* _driver);
	// destroctor
	~VertexShader();
};

// pixel shader class for pixel shader data and operations
class PixelShader
{
	std::string name;

	ID3D11PixelShader* shader;

	// shader reflection data
	std::vector<ConstantBuffer> constBuffer;
	std::map<std::string, int> texBindPoints;

	// compile pixel shader
	void CompilePixelShader(std::string _shader, DXCore* _driver);
public:
	PixelShader(std::string _name, std::string _location, DXCore* _driver);
	// update constant buffer variable
	void UpdateConstant(std::string _bufferName, std::string _varName, void* _data);
	// update texture in shader
	void UpdateTexture(std::string _name, ID3D11ShaderResourceView* srv, DXCore* _driver);
	// apply shader
	void Apply(DXCore* _driver);
	// destructor
	~PixelShader();
};

// preloades shaders and stores it
static class ShaderManager
{
	static std::map<std::string, VertexShader*> vertexShaders; // store vertex shaders
	static std::map<std::string, PixelShader*> pixelShaders;// store pixel shaders

	static DXCore* driver; // reference to the device
	static std::string current[2]; // current applied shader 0 for vertex and 1 for pixel

	ShaderManager() = default;
public:
	static bool lockVertex;
	static bool lockPixel;

	static void Init(DXCore* _driver);

	// add vertex shader to list
	static void AddVertex(std::string _name, std::string _location, LayoutType _type = LayoutType::Normal);
	// add vertex shader to list
	static void AddPixel(std::string _name, std::string _location);

	// set vertex shader
	static void SetVertex(std::string _name);
	// set pixel shader
	static void SetPixel(std::string _name);
	// set combination of shaders
	static void Set(std::string _vName, std::string _pName);

	// apply shader of given name
	static void ApplyVertex();
	// apply shader of given name
	static void ApplyPixel();
	// apply shader of given name
	static void Apply();

	// update shader constant by shader name
	static void UpdateVertex(std::string _name, std::string _bufferName, std::string _varName, void* _data);
	// update shader constant by shader name
	static void UpdateVertex(std::string _bufferName, std::string _varName, void* _data);
	// update shader constant by shader name
	static void UpdatePixel(std::string _bufferName, std::string _varName, void* _data);
	// update constant of a shader with given name
	static void UpdateAll(ShaderStage _type, std::string _bufferName, std::string _varName, void* _data);

	// update texture
	static void UpdatePixel(std::string _varName, ID3D11ShaderResourceView* _srv);
	// update texture of perticular shader
	static void UpdatePixel(std::string _name, std::string _varName, ID3D11ShaderResourceView* _srv);

	// destroy all shaders
	static void Free();
};

