#pragma once
#include "GMath.h"
#include "Driver.h"

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

class Behaviour
{
protected:
	Behaviour(Vec3 _pos = Vec3::zero, Vec3 _rot = Vec3::zero, Vec3 _scale = Vec3::one);
public:
	Transform transform;

	virtual void Update(float _dt) {};
	virtual void Draw() {};

	void Translate(Vec3 _vel);
	void TranslateRel(Vec3 _vel);
	void Rotate(Vec3 _angle);
};
