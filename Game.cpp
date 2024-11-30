#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include <sstream>

const unsigned int WIDTH = 1080;
const unsigned int HEIGHT = 768;

//int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
int main()
{
	Window win;
	Timer timer;

	Camera camera(Vec2(WIDTH, HEIGHT), Vec3(0, 5, 10), Vec3(0, 0, 0));


	win.Create(WIDTH, HEIGHT, "My Window", false, 100, 0);
	DXCore& device = win.GetDevice();

	ShaderManager::Init(device);
	ShaderManager::Add("1", "Shaders/StaticMeshVertexShader.hlsl", "Shaders/StaticMeshPixelShader.hlsl");
	ShaderManager::Add("2", "Shaders/AnimatedMeshVertexShader.hlsl", "Shaders/StaticMeshPixelShader.hlsl", true);


	Plane plane(device);
	Matrix planeWorld = Matrix();

	/*Cube cube(device);
	Matrix cubeWorld = Matrix::Translation(Vec3(0, 1, 0));

	Sphere sphere(10, 10, 1, device);
	Matrix sphereWorld = Matrix::Translation(Vec3(2, 1, 0));*/

	StaticMesh tree("Models/TRex/acacia_003.gem", device);
	Matrix treeWorld = Matrix::Translation(Vec3(0, 0, -10)) * Matrix::Scaling(Vec3(0.01f, 0.01f, 0.01f));

	AnimatedMesh tRex("Models/TRex/TRex.gem", device);
	Matrix tRexWorld;// Matrix::Scaling(Vec3(5, 5, 5));;
	AnimationInstance instance;
	instance.animation = &tRex.animation;

	win.inputs.SetCursorLock(true);
	float dt, moveSpeed, rotSpeed = 50;

	while (true)
	{
		win.Update();

		dt = timer.dt();

		Vec2 moveDelta = win.inputs.GetAxis() * dt;
		Vec2 rotDelta = -win.inputs.MouseDelta() * dt;

		moveSpeed = win.inputs.KeyPressed(VK_SHIFT) ? 100 : 10;

		if (moveDelta.Length() > 0)
			camera.TranslateRel(Vec3(moveDelta.x, 0, moveDelta.y) * moveSpeed);
		if (rotDelta.Length() > 0)
			camera.Rotate(Vec3(rotDelta.y, rotDelta.x, 0) * rotSpeed);

		Matrix vp = camera.GetViewProjMat();
		instance.update("Run", dt);

		win.Clear();

		ShaderManager::Set("1");
		ShaderManager::UpdateConstant(ShaderStage::VertexShader, "staticMeshBuffer", "VP", &vp);

		ShaderManager::UpdateConstant(ShaderStage::VertexShader, "staticMeshBuffer", "W", &planeWorld);
		ShaderManager::Apply();
		plane.Draw(device);

		/*ShaderManager::UpdateConstant("1", ShaderStage::VertexShader, "staticMeshBuffer", "W", &cubeWorld);
		ShaderManager::Apply("1");
		cube.Draw(device);

		ShaderManager::UpdateConstant("1", ShaderStage::VertexShader, "staticMeshBuffer", "W", &sphereWorld);
		ShaderManager::Apply("1");
		sphere.Draw(device);*/

		ShaderManager::Set("4");

		ShaderManager::UpdateConstant(ShaderStage::VertexShader, "staticMeshBuffer", "W", &treeWorld);
		ShaderManager::Apply();
		tree.Draw(device);

		ShaderManager::Set("2");
		ShaderManager::UpdateConstant(ShaderStage::VertexShader, "animatedMeshBuffer", "VP", &vp);
		ShaderManager::UpdateConstant(ShaderStage::VertexShader, "animatedMeshBuffer", "W", &tRexWorld);
		ShaderManager::UpdateConstant(ShaderStage::VertexShader, "animatedMeshBuffer", "bones", &instance.matrices);
		ShaderManager::Apply();
		tRex.Draw(device);

		win.Present();

		if (win.inputs.KeyPressed(VK_ESCAPE))
			break;
	}

	return 0;
}