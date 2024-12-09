#pragma once
#include "Collisions.h"
#include <random>
#include "Shader.h"
#include "Texture.h"
#include "Window.h"
#include "Camera.h"

class Particles
{
	InstancedMeshData mesh;
	std::vector<Vec3> positions;

	Matrix worldMat;
	DXCore* driver;
	Camera* camera;
	float time; // time for vertex animations
	float height;

public:
	Particles() = default;
	void Init(Vec3 _volume, Vec3 _pos, unsigned int _total, DXCore* _driver);
	void Update(float _dt);
	void Draw();
};

class Grass
{
	InstancedMeshData mesh;
	std::vector<Vec3> positions;

	Matrix worldMat;
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
	Plane plane;
	Vec2 tiling;

	DXCore* driver;
public:
	Ground();
	void Draw();
	~Ground();
};

class Box :public Collider
{
	Cube box;
	DXCore* driver;
public:
	Box() = default;
	void Init(Vec3 _pos, DXCore* _driver);
	void Draw();
	~Box();
};

class Level
{
	DXCore* driver;

	Sphere sky;;
	Matrix skyWorld;

	Ground ground;
	Grass grass;

	Box box;
	Box staticObject;

	Trees trees;
	Particles particles;

	float time = 0;
public:
	Level(DXCore* _driver);
	void Update(float _dt);
	void Draw();
};
