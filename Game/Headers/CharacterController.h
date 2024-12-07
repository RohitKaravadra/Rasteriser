#pragma once
#include "Behaviour.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Window.h"
#include "Collisions.h"

class CharacterController : public Behaviour
{
	AnimatedMesh visuals;
	AnimationInstance instance;

	Camera* camera;
	Window* win;
	DXCore* driver;

	bool isGrounded;
	bool colliding;
	float gravity;
	bool attack;

	float moveSpeed;
	float rotSpeed;
public:

	CharacterController(Vec3 _pos, Vec3 _rot, Vec3 _scale);

	void Update(float _dt) override;
	void Draw() override;
	void OnCollision(const Collider& _other) override;
	~CharacterController();
};
