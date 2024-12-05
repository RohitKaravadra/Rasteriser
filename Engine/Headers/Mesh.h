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

static STATIC_VERTEX addVertex(Vec3 p, Vec3 n, float tu, float tv);

class MeshData
{
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* vertexBuffer;
	int indicesSize;
	UINT strides;

	void Init(void* vertices, int vertexSizeInBytes, int numVertices,
		unsigned int* indices, int numIndices, DXCore* _driver);

public:
	// create static mesh with given vertices and indices
	void Init(std::vector<STATIC_VERTEX> vertices, std::vector<unsigned int> indices, DXCore* _driver);
	// create animated mesh with given vertices and indices
	void Init(std::vector<ANIMATED_VERTEX> vertices, std::vector<unsigned int> indices, DXCore* _driver);
	// draw mesh
	void Draw(DXCore* _driver) const;
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
	void AddData(std::string _texture, MeshData _mesh);
public:
	std::map<std::string, std::vector<MeshData>> data;
	std::vector<std::string> textureFilenames;

	virtual void Init(std::string _location, DXCore* _driver);
	void Draw(DXCore* _driver);
};

class AnimatedMesh :public Mesh
{
public:
	Animation animation;

	void Init(std::string _location, DXCore* _driver) override;
};


