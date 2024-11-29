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

class Mesh
{
public:
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* vertexBuffer;
	int indicesSize;
	UINT strides;

	Mesh() = default;

	void Init(void* vertices, int vertexSizeInBytes, int numVertices, unsigned int* indices, int numIndices, DXCore& _driver) {
		D3D11_BUFFER_DESC bd;
		memset(&bd, 0, sizeof(D3D11_BUFFER_DESC));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * numIndices;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		D3D11_SUBRESOURCE_DATA data;
		memset(&data, 0, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = indices;
		_driver.device->CreateBuffer(&bd, &data, &indexBuffer);
		bd.ByteWidth = vertexSizeInBytes * numVertices;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		data.pSysMem = vertices;
		_driver.device->CreateBuffer(&bd, &data, &vertexBuffer);
		indicesSize = numIndices;
		strides = vertexSizeInBytes;
	}

	// create static mesh with given vertices and indices
	void Init(std::vector<STATIC_VERTEX> vertices, std::vector<unsigned int> indices, DXCore& _driver);
	// create animated mesh with given vertices and indices
	void Init(std::vector<ANIMATED_VERTEX> vertices, std::vector<unsigned int> indices, DXCore& _driver);
	// draw mesh
	void Draw(DXCore& _driver) const;
};

class Plane
{
	Mesh mesh;
public:

	Plane(DXCore& _driver);
	void Draw(DXCore& _driver);
};

class Cube
{
	Mesh mesh;
public:

	Cube(DXCore& _driver);
	void Draw(DXCore& _driver);
};

class Sphere
{
	Mesh mesh;
public:

	Sphere(unsigned int rings, unsigned int segments, unsigned int radius, DXCore& _driver);
	void Draw(DXCore& _driver);
};

class StaticMesh
{
	std::vector<Mesh> meshes;
public:
	StaticMesh(std::string _location, DXCore& _driver);
	void Draw(DXCore& _driver);
};

class AnimatedMesh
{
	std::vector<Mesh> meshes;

public:
	Animation animation;

	AnimatedMesh(std::string _location, DXCore& _driver);
	void Draw(DXCore& _driver);
};


