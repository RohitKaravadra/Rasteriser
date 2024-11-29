#pragma once
#include "GMath.h"
#include "Driver.h"

class Behaviour
{
	void OnScale();
	void OnRotate();
	void OnTranslate();
protected:
	Vec3 pos;
	Vec3 scale;
	Vec3 rot;

	Vec3 forward;
	Vec3 right;
	Vec3 up;

	Matrix worldMat;

	Behaviour(Vec3 _pos = Vec3::zero, Vec3 _rot = Vec3::zero, Vec3 _scale = Vec3::one);
public:
	Vec3 Position() const;
	Vec3 Rotation() const;
	Vec3 Scale() const;

	Vec3 Forward() const;
	Vec3 Right() const;
	Vec3 Up() const;

	void Position(Vec3 _pos);
	void Rotation(Vec3 _rot);
	void Scale(Vec3 _scale);

	void Translate(Vec3 _vel);
	void TranslateRel(Vec3 _vel);
	void Rotate(Vec3 _angle);

	virtual void OnWorldUpdate() {};

	virtual void Update(float _dt) {};
	virtual void Draw() {};
};

class StaticBehaviour
{
protected:
	Vec3 pos;
	Vec3 scale;
	Vec3 rot;

	Vec3 forward;
	Vec3 right;
	Vec3 up;

	Matrix worldMat;

	StaticBehaviour(Vec3 _pos = Vec3::zero, Vec3 _rot = Vec3::zero, Vec3 _scale = Vec3::one);
public:
	Vec3 Position() const;
	Vec3 Rotation() const;
	Vec3 Scale() const;

	Vec3 Forward() const;
	Vec3 Right() const;
	Vec3 Up() const;

	virtual void Update(float _dt) {};
	virtual void Draw() {};
};