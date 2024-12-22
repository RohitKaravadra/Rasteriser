#include "Camera.h"
#include "Window.h"

Camera* Camera::instance = nullptr;

Camera::Camera(Vec2 _size, Vec3 _pos, Vec3 _rot, float _nearPlane, float _farPlane) :Behaviour(_pos, _rot)
{
	if (instance != nullptr)
		return;

	projMat = Matrix4x4::PerProject(45, _size.x / _size.y, _nearPlane, _farPlane);
	//projMat = Matrix::OrthoProject(_size.x/2, _size.y/2, _nearPlane, _farPlane);

	UpdateMat();
	rotSpeed = 40;
	moveSpeed = 20;
	instance = this;
}

Camera* Camera::GetInstance() { return instance; }

void Camera::UpdateMat()
{
	viewMat = Matrix4x4::View(transform.worldMat);
	viewProj = projMat * viewMat;
}

void Camera::Update(float _dt)
{
	Window* win = Window::GetInstance();

	if (!win->inputs.GetCursorLock())
		return;

	moveSpeed = clamp(moveSpeed + win->inputs.MouseWheel() * 10 * _dt, 1, 2000);

	Vec2 moveDelta = win->inputs.GetAxis() * moveSpeed * _dt;
	Vec2 rotDelta = -win->inputs.MouseDelta() * rotSpeed * _dt;

	if (moveDelta.Length() > 0)
		TranslateRel(Vec3(moveDelta.x, 0, moveDelta.y));
	if (rotDelta.Length() > 0)
		Rotate(Vec3(rotDelta.y, rotDelta.x, 0) % 20.f);

	UpdateMat();
}

Matrix4x4 Camera::GetViewProjMat() const { return viewProj; }
Matrix4x4 Camera::GetViewMat() const { return viewMat; }

Camera::~Camera() { if (instance == this)instance = nullptr; }