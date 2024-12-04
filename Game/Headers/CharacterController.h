#pragma once
#include "Behaviour.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Window.h"

class CharacterController : public Behaviour
{
	AnimatedMesh visuals;
	AnimationInstance instance;

	Camera* camera;
	Window* win;
	DXCore* driver;

	float moveSpeed;
	float rotSpeed;

public:

	CharacterController(Vec3 _pos, Vec3 _rot, Vec3 _scale);

	void Update(float _dt) override;
	void Draw() override;
};
