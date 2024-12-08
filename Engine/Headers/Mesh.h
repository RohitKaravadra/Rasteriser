#pragma once
#include "Animation.h"
#include<Driver.h>

struct STATIC_VERTEX
{
	Vec3 pos;
	Vec3 normal;
	Vec3 tangent;
	float tu;
	float tv;
};

struct ANIMATED_VERTEX
{
	Vec3 pos;
	Vec3 normal;
	Vec3 tangent;
	float tu;
	float tv;
	unsigned int bonesIDs[4];
	float boneWeights[4];
};

struct INSTANCE_DATA
{
	Vec3 pos;
};

static STATIC_VERTEX addVertex(Vec3 p, Vec3 n, float tu, float tv);

class MeshData
{
protected:
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* vertexBuffer;
	int indicesSize;
	UINT strides;

	void Init(void* vertices, int vertexSizeInBytes, int numVertices,
		unsigned int* indices, int numIndices, DXCore* _driver);

public:
	MeshData();
	MeshData(const MeshData& other);
	// create static mesh with given vertices and indices
	void Init(std::vector<STATIC_VERTEX> vertices, std::vector<unsigned int> indices, DXCore* _driver);
	// create animated mesh with given vertices and indices
	void Init(std::vector<ANIMATED_VERTEX> vertices, std::vector<unsigned int> indices, DXCore* _driver);
	// draw mesh
	virtual void Draw(DXCore* _driver) const;
	// copy data of one object to another to avoid double deletion of pointers
	void Copy(const MeshData& other);
	// free all data
	void Free();
	// Assignment Operator
	MeshData& operator=(const MeshData& other);
	// distructor
	~MeshData();
};

class InstancedMeshData :public MeshData
{
	ID3D11Buffer* instanceBuffer;

	unsigned int instanceSize;
	unsigned int instancesSize;

public:
	InstancedMeshData();
	InstancedMeshData(const InstancedMeshData& _other);

	void Copy(const InstancedMeshData& _other);
	void Copy(const MeshData& _other);
	void SetInstanceData(unsigned int _instanceSize, unsigned int _instancesSize, void* _buffer, DXCore* _driver);
	void UpdateBuffer(void* _buffer);
	void Draw(DXCore* _driver) const override;
	void Free();
	InstancedMeshData& operator=(const InstancedMeshData& _other);
	~InstancedMeshData();
};

class Plane
{
	MeshData mesh;
public:
	Plane() = default;
	Plane(DXCore* _driver);
	void Draw(DXCore* _driver);
};

class Cube
{
	MeshData mesh;
public:
	Cube() = default;
	Cube(DXCore* _driver);
	void Draw(DXCore* _driver);
};

class Sphere
{
	MeshData mesh;
public:
	Sphere() = default;
	Sphere(unsigned int rings, unsigned int segments, unsigned int radius, DXCore* _driver);
	void Draw(DXCore* _driver);
};

class Mesh
{
protected:
	void AddData(std::string _texture, std::string _normal, MeshData _mesh);
public:
	std::vector<std::vector<MeshData>> meshes; // map of texture file name and all meshes
	std::vector<std::string> textureFiles;
	std::vector<std::string> normalFiles;

	virtual void Init(std::string _location, DXCore* _driver, std::string _textLocation = "\0");
	void Draw(DXCore* _driver);
	void PrintTextures();
};

class AnimatedMesh :public Mesh
{
public:
	Animation animation;

	void Init(std::string _location, DXCore* _driver, std::string _textLocation = "\0") override;
};

class InstancedMesh
{
protected:
	void AddData(std::string _texture, std::string _normal, InstancedMeshData _mesh);
public:
	std::vector<std::vector<InstancedMeshData>> meshes; // map of texture file name and all meshes
	std::vector<std::string> textureFiles;
	std::vector<std::string> normalFiles;

	void Init(std::string _location, DXCore* _driver, std::string _textLocation = "\0");
	void SetInstanceData(unsigned int _instanceSize, unsigned int _instancesSize, void* _buffer, DXCore* _driver);
	void Draw(DXCore* _driver);
	void PrintTextures();
};

class Billboard
{
public:
	MeshData mesh;
	void Init(DXCore* _driver);
	void Draw(DXCore* _driver);
};




