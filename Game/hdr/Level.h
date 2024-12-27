#pragma once
#include "../../Engine/hdr/Collisions.h"
#include "../../Engine/hdr/Shader.h"
#include "../../Engine/hdr/Texture.h"
#include "../../Engine/hdr/Window.h"
#include "../hdr/Camera.h"
#include <random>

class Particles
{
	InstancedMeshData mesh;
	std::vector<Vec3> positions;

	DXCore* driver;
	Camera* camera;

	float time; // time for vertex animations
	float height;

public:
	Particles() = default;
	void Init(Vec3 _volume, unsigned int _total, DXCore* _driver);
	void Update(float _dt);
	void Draw();
};

class Grass
{
	InstancedMeshData mesh;
	std::vector<Vec3> positions;

	DXCore* driver;
	Camera* camera;
	float time; // time for vertex animations

public:
	Grass() = default;
	void Init(Vec2 _area, unsigned int _total, DXCore* _driver);
	void Update(float _dt);
	void Draw();
};

// creating tress using instancing
class Trees
{
	std::vector<InstancedMesh> meshes;
	std::vector<std::vector<Vec3>> positions;

	Matrix worldMat;
	DXCore* driver;
	float time; // time for vertex animations

public:
	Trees() = default;
	void Init(unsigned int _total, DXCore* _driver);
	void Update(float _dt);
	void Draw();
};

class Ground :public Collider
{
	MeshData* plane;
	Vec2 tiling;

	DXCore* driver;
public:
	Ground(DXCore* _driver);
	void Draw();
	~Ground();
};

class Sky
{
	MeshData* sphere;
	Matrix worldMat;
	DXCore* driver;
public:
	Sky(DXCore* _driver);
	void Rotate(float _angle);
	void Draw();
	~Sky();
};

class Box :public Collider
{
	MeshData* box;
	DXCore* driver;
public:
	Box(Vec3 _pos, DXCore* _driver);
	void Draw();
	~Box();
};

class Level
{
	DXCore* driver;

	Grass grass;

	Box* box;
	Box* staticObject;

	Trees trees;
	Particles particles;

	float time = 0;
public:
	Level(DXCore* _driver);
	void Update(float _dt);
	void Draw();
	~Level();
};
