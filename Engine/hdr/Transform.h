#pragma once
#include "GMath.h"

struct Transform
{
	Vec3 position;
	Vec3 scale;
	Vec3 rotation;

	Vec3 forward;
	Vec3 right;
	Vec3 up;

	Matrix worldMat;

	Transform();
	Transform(Vec3 _pos, Vec3 _rot, Vec3 _scale);
	void Update();
	friend std::ostream& operator<<(std::ostream& os, const Transform& transform);
};
