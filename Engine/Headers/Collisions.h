#pragma once
#include "Mesh.h"
#include "Transform.h"

class Collider
{
protected:
	// resolve collision on given data
	void Resolve(Vec3 _dist, Vec3 _delta, float _push);

public:
	bool isStatic; // can moved by other objects
	float mass; // mass for movement weight
	bool isEnabled; // enables gizmos for debugging

	Vec3 size; // size of collider
	Vec3 offset; // offset of collider relative to object transform

	std::string tag; // tag of this collider to identify it

	Transform transform; // transform of this collider

	Collider();
	// check collision and resolve if needed
	bool Collide(Collider& _other);
	// draw gizmos
	void Draw(Cube& gizmo, DXCore* _driver);

	// virtual functions as callback when collision occurs
	virtual void OnCollision(const Collider& _other) {};
};

// handles all collider and their collision
static class Collisions
{
	// colliders list
	static std::vector<Collider*> colliders;

	static DXCore* driver;
	// gizmos object for debugging
	static Cube cubeGizmo;
public:
	// set driver
	static void Init(DXCore* _driver);
	// add collider to a layer
	static void AddCollider(Collider* _collider);
	// remove collider
	static void RemoveCollider(Collider* _collider);
	// update collisions
	static void Update();
	// draw all collider gizmos
	static void DrawGizmos();
};