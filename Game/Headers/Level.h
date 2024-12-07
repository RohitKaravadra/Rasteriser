#pragma once
#include "Collisions.h"
#include <random>
#include "Shader.h"
#include "Texture.h"
#include "Window.h"

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
