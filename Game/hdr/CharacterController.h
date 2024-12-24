#pragma once
#include "../../Engine/hdr/Behaviour.h"
#include "../../Engine/hdr/Mesh.h"
#include "../../Engine/hdr/Shader.h"
#include "../../Engine/hdr/Texture.h"
#include "../../Engine/hdr/Window.h"
#include "../../Engine/hdr/Collisions.h"
#include "../hdr/Camera.h"

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
