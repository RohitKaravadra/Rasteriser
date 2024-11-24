#pragma once
#include "GMath.h"
#include<Drivers.h>

struct Vertex
{
	Vec3 position;
	Color colour;
};

class Triangle
{
public:
	Vertex vertices[3];
	ID3D11Buffer* vertexBuffer; // buffer for graphic device

	// initialize triangle and vertex buffer
	void Init(DXCore& _driver, int _N);
	// draw triangle
	void Draw(DXCore& _driver) const;
};
