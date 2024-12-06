#include "CharacterController.h"

CharacterController::CharacterController(Vec3 _pos, Vec3 _rot, Vec3 _scale) :Behaviour(_pos, _rot, _scale)
{
	win = Window::GetInstance();
	driver = &win->GetDevice();
	camera = Camera::GetInstance();

	camera->transform.rotation = transform.rotation;
	camera->transform.position = transform.position + Vec3(0, 4, 0) - camera->transform.forward * 20;
	camera->transform.Update();
	camera->UpdateMat();

	visuals.Init("Resources/TRex/TRex.gem", driver);
	instance.animation = &visuals.animation;

	moveSpeed = 10;
	rotSpeed = 20;
}

void CharacterController::Update(float _dt)
{
	bool camDirty = false; // flag to check camera is updated or not

	// set move speed
	moveSpeed = Lerp(moveSpeed, win->inputs.KeyPressed(VK_SHIFT) ? 20.f : 10.f, 2 * _dt);

	// get inputs
	Vec2 moveDelta = win->inputs.GetAxis() * moveSpeed * _dt;
	Vec2 rotDelta = win->inputs.GetCursorLock() ? -win->inputs.MouseDelta() * rotSpeed * _dt : Vec2(0, 0);

	if (rotDelta.Length() > 0)
	{
		//update camera rotation
		Vec3 clampRot = camera->transform.rotation;
		clampRot.x = clamp(clampRot.x + rotDelta.y, -60, 10);
		clampRot.y += rotDelta.x;

		camera->transform.rotation = clampRot;
		camera->transform.Update();
		camDirty = true;
	}

	if (moveDelta.Length() > 0)
	{
		// update character movement and rotation
		transform.rotation = Lerp(transform.rotation, Vec3(0, camera->transform.rotation.y, 0), 2 * _dt);
		TranslateRel(Vec3(moveDelta.x, 0, moveDelta.y));

		camDirty = true;
	}

	// update animations
	if (moveDelta.Length() > 0)
		instance.update("Run", _dt * (moveDelta.y < 0 ? -1 : 1) * moveSpeed / 10);
	else
		instance.update("Idle", _dt);

	// update camera matrices
	if (camDirty)
	{
		// update camera movement
		camera->transform.position = transform.position + Vec3(0, 4, 0) - camera->transform.forward * 20;
		camera->transform.Update();
		camera->UpdateMat();
	}
}

void CharacterController::Draw()
{
	Matrix vp = camera->GetViewProjMat();

	ShaderManager::Set("TRex");
	ShaderManager::UpdateConstant(ShaderStage::VertexShader, "ConstBuffer", "VP", &vp);
	ShaderManager::UpdateConstant(ShaderStage::VertexShader, "ConstBuffer", "W", &transform.worldMat);
	ShaderManager::UpdateConstant(ShaderStage::VertexShader, "ConstBuffer", "bones", &instance.matrices);

	ShaderManager::UpdateTexture(ShaderStage::PixelShader, "tex", TextureManager::find("T-rex_Base_Color.png"));

	ShaderManager::Apply();
	visuals.Draw(driver);
}
