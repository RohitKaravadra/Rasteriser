#pragma once
#include "Driver.h"
#include <map>
#include "GMath.h"
#include "ShaderReflection.h"

// read data from a file and return data in string format
static std::string GetFileData(std::string fileName);

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

public:
	// create and compile shader
	void Init(std::string _VSLocation, std::string _PSLocation, DXCore& _driver);
	// compile vertex shader and bind layout
	void CompileVertexShader(std::string _shader, DXCore& _driver);
	// compile pixel shader
	void CompilePixelShader(std::string _shader, DXCore& _driver);
	// apply shader
	void ApplyShader(DXCore& _driver);
	// update the value inside the constant buffer
	void updateConstant(std::string constantBufferName, std::string variableName, void* data, std::vector<ConstantBuffer>& buffers)
	{
		for (int i = 0; i < buffers.size(); i++)
		{
			if (buffers[i].name == constantBufferName)
			{
				buffers[i].update(variableName, data);
				return;
			}
		}
	}
	// update vertex constant buffer
	void updateConstantVS(std::string constantBufferName, std::string variableName, void* data)
	{
		updateConstant(constantBufferName, variableName, data, vsConstantBuffers);
	}
	// update pixel constant buffer
	void updateConstantPS(std::string constantBufferName, std::string variableName, void* data)
	{
		updateConstant(constantBufferName, variableName, data, psConstantBuffers);
	}
};

// preloades shaders and stores it
static class ShaderManager
{
	static std::map<std::string, Shader> shaders;
public:
	static void Load(std::vector<std::string>& _shaders, DXCore& _driver);
	static bool GetShader(std::string _name, Shader& _shader);
};
