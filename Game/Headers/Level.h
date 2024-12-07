#pragma once
#include "Collisions.h"
#include <random>
#include "Shader.h"
#include "Texture.h"
#include "Window.h"

// creating tress using instancing
class Trees
{
	std::vector<Mesh> meshes;
	std::vector<Matrix> worldMats;
	DXCore* driver;

	float time;
public:
	Trees() = default;
	void Init(DXCore* _driver);
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

class CustomMesh :public Collider
{
	Mesh mesh;
	DXCore* driver;

public:
	CustomMesh() = default;
	void Init(Vec3 _pos, DXCore* _driver);
	void Draw();
	~CustomMesh();
};

class Level
{
	DXCore* driver;

	Ground ground;
	Trees trees;
	Box box;
	Box object;

	Sphere sky;;
	Matrix skyWorld;
public:
	Level(DXCore* _driver);
	void Update(float _dt);
	void Draw();
};
