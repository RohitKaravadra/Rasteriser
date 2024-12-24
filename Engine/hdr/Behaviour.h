#pragma once
#include "Collisions.h"

// simple behaviour class for object creation
class Behaviour :public Collider
{
protected:
	Behaviour(Vec3 _pos = Vec3::zero, Vec3 _rot = Vec3::zero, Vec3 _scale = Vec3::one);
public:

	// virtual function should be overriden by inheriting objects
	virtual void Update(float _dt) {};
	virtual void Draw() {};

	// translate relative to world
	void Translate(Vec3 _vel);
	// translate relative to local
	void TranslateRel(Vec3 _vel);
	// Rotate object in local space
	void Rotate(Vec3 _angle);
};
