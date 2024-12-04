#include "CharacterController.h"

CharacterController::CharacterController(Vec3 _pos, Vec3 _rot, Vec3 _scale)
{
	win = Window::GetInstance();
	driver = &win->GetDevice();
	camera = Camera::GetInstance();

	camera->Rotation(transform.rot);
	camera->Position(transform.pos + Vec3(0, 4, 0) - camera->Forward() * 20);

	visuals.Init("Models/TRex/TRex.gem", driver);
	instance.animation = &visuals.animation;

	moveSpeed = 10;
	rotSpeed = 10;
}

void CharacterController::Update(float _dt)
{
	// set move speed
	moveSpeed = win->inputs.KeyPressed(VK_SHIFT) ? 100 : 10;

	// get inputs
	Vec2 moveDelta = win->inputs.GetAxis() * moveSpeed * _dt;
	Vec2 rotDelta = win->inputs.GetCursorLock() ? -win->inputs.MouseDelta() * rotSpeed * _dt : Vec2(0, 0);

	// update movement and rotation
	if (moveDelta.Length() > 0)
		TranslateRel(Vec3(moveDelta.x, 0, moveDelta.y));
	if (rotDelta.Length() > 0)
		Rotate(Vec3(0, rotDelta.x, 0));

	// update animations
	if (moveDelta.Length() > 0)
		instance.update("Run", _dt * (moveDelta.y < 0 ? -1 : 1));
	else
		instance.update("Idle", _dt);

	// update camera
	camera->Rotate(Vec3(rotDelta.y, rotDelta.x, 0));
	camera->Position(transform.pos + Vec3(0, 4, 0) - camera->Forward() * 20);
}

void CharacterController::Draw()
{
	Matrix vp = camera->GetViewProjMat();

	ShaderManager::Set("TRex");
	ShaderManager::UpdateConstant(ShaderStage::VertexShader, "AnimConstBuffer", "VP", &vp);
	ShaderManager::UpdateConstant(ShaderStage::VertexShader, "AnimConstBuffer", "W", &transform.worldMat);
	ShaderManager::UpdateConstant(ShaderStage::VertexShader, "AnimConstBuffer", "bones", &instance.matrices);

	ShaderManager::UpdateTexture(ShaderStage::PixelShader, "tex", TextureManager::find("T-rex_Base_Color.png"));

	ShaderManager::Apply();
	visuals.Draw(driver);
}
