#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Timer.h"
#include "Camera.h"
#include <sstream>

const unsigned int WIDTH = 768;
const unsigned int HEIGHT = 768;

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
//int main()
{
	Window win;
	Shader shader;

	Cube cube;
	Sphere sphere;
	Plane plane;

	Timer timer;

	Camera camera(Vec3(0, 0, 0), Vec3(0, 0, -1));

	Matrix projMat = Matrix::Projection(45, WIDTH / HEIGHT, 0.1f, 200.f);
	Matrix vp;

	Matrix planeWorld = Matrix::Translation(Vec3(0, -1, 0));
	Matrix cubeWorld = Matrix::Translation(Vec3(0, 1, 0));
	Matrix sphereWorld = Matrix::Translation(Vec3(2, 1, 0));

	win.Create(WIDTH, HEIGHT, "My Window", false, 100, 0);
	DXCore& device = win.GetDevice();

	shader.Init("Shaders/StaticMeshVertexShader.hlsl", "Shaders/StaticMeshPixelShader.hlsl", device);

	cube.Init(device);
	plane.Init(device);
	sphere.Init(device, 10, 10, 1);

	float dt, moveSpeed = 10;

	while (true)
	{
		dt = timer.dt();

		Vec2 moveDelta = win.inputs.GetAxis() * dt;
		Vec2 rotDelta = -win.inputs.GetAxis2() * dt;

		if (moveDelta.Length() > 0)
			camera.Move(Vec3(moveDelta.x, 0, moveDelta.y) * moveSpeed);
		if (rotDelta.Length() > 0)
			camera.Rotate(rotDelta.x, rotDelta.y);

		vp = projMat.Mul(camera.ViewMat());

		win.Update();
		win.Clear();

		shader.updateConstantVS("staticMeshBuffer", "VP", &vp);

		shader.updateConstantVS("staticMeshBuffer", "W", &planeWorld);
		shader.ApplyShader(device);
		plane.mesh.Draw(device);

		shader.updateConstantVS("staticMeshBuffer", "W", &cubeWorld);
		shader.ApplyShader(device);
		cube.mesh.Draw(device);

		shader.updateConstantVS("staticMeshBuffer", "W", &sphereWorld);
		shader.ApplyShader(device);
		sphere.mesh.Draw(device);

		win.Present();
	}

	return 0;
}