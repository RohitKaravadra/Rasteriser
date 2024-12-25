#pragma once
#include "Mesh.h"

enum Gizmo {Sphere, Cube};

static class Gizmos
{
	static DXCore* driver;
	static MeshData shapes[2]; // 0 = sphere, 1 = cube
public:
	static void Init(DXCore* _driver);
	// set gizmos draw mode
	static void Set();
	// draw gizmo
	static void Draw(Gizmo _type, Matrix _mat);
	// draw gizmo
	static void Draw(Gizmo _type, Vec3 _pos);
	// reset gizmos draw mode
	static void Reset();
};