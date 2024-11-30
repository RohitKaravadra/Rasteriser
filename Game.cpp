#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include <sstream>
#include <random>

const unsigned int WIDTH = 1080;
const unsigned int HEIGHT = 768;

class Trees
{
	StaticMesh mesh;
	std::vector<Matrix> worldMats;
	DXCore& driver;
public:
	Trees(unsigned int _total, DXCore& _driver) : driver(_driver)
	{
		mesh.Init("Models/TRex/acacia_003.gem", driver);

		std::random_device rd;
		std::mt19937 rGen(rd());
		std::uniform_int_distribution<> posDistr(-300, 300);
		std::uniform_real_distribution<> scalDistr(0.01f, 0.05f);

		for (unsigned int i = 0; i < _total; i++)
		{
			Matrix mat = Matrix::Translation(Vec3(posDistr(rGen), 0, posDistr(rGen))) * Matrix::Scaling(scalDistr(rGen));
			worldMats.push_back(mat);
		}
	}
	void Draw()
	{
		ShaderManager::Set("1");
		for (Matrix& worldMat : worldMats)
		{
			ShaderManager::UpdateConstant(ShaderStage::VertexShader, "staticMeshBuffer", "W", &worldMat);
			ShaderManager::Apply();
			mesh.Draw(driver);
		}
	}
};

class TRex :public StaticBehaviour
{
	AnimatedMesh mesh;
	AnimationInstance instance;
	DXCore& driver;
public:
	TRex(Vec3 _pos, Vec3 _rot, Vec3 _scale, DXCore& _driver) : driver(_driver), StaticBehaviour(_pos, _rot, _scale)
	{
		mesh.Init("Models/TRex/TRex.gem", driver);
		instance.animation = &mesh.animation;
	}

	void Update(float _dt) override
	{
		instance.update("Idle", _dt);
	}

	void Draw() override
	{
		ShaderManager::Set("2");
		ShaderManager::UpdateConstant(ShaderStage::VertexShader, "animatedMeshBuffer", "W", &transform.worldMat);
		ShaderManager::UpdateConstant(ShaderStage::VertexShader, "animatedMeshBuffer", "bones", &instance.matrices);
		ShaderManager::Apply();
		mesh.Draw(driver);
	}
};

//int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
int main()
{
	Window win;
	Timer timer;

	Camera camera(Vec2(WIDTH, HEIGHT), Vec3(0, 5, 10), Vec3(0, 0, 0));


	win.Create(WIDTH, HEIGHT, "My Window", true, 100, 100);
	DXCore& device = win.GetDevice();

	ShaderManager::Init(device);
	ShaderManager::Add("1", "Shaders/StaticMeshVertexShader.hlsl", "Shaders/StaticMeshPixelShader.hlsl");
	ShaderManager::Add("2", "Shaders/AnimatedMeshVertexShader.hlsl", "Shaders/StaticMeshPixelShader.hlsl", true);

	Trees trees(500, device);

	Plane plane(device);
	Matrix planeWorld = Matrix::Scaling(Vec3(50));

	Sphere sky(50, 50, 500, device);
	Matrix skyWorld = Matrix::Identity();

	TRex tRex(Vec3(0, 0, 0), Vec3(0, 180, 0), Vec3::one, device);

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
		//tRex.Update(dt);

		win.Clear();


		ShaderManager::Set("2");
		ShaderManager::UpdateConstant(ShaderStage::VertexShader, "animatedMeshBuffer", "VP", &vp);

		ShaderManager::Set("1");
		ShaderManager::UpdateConstant(ShaderStage::VertexShader, "staticMeshBuffer", "VP", &vp);

		ShaderManager::UpdateConstant(ShaderStage::VertexShader, "staticMeshBuffer", "W", &planeWorld);
		ShaderManager::Apply();
		plane.Draw(device);

		ShaderManager::UpdateConstant(ShaderStage::VertexShader, "staticMeshBuffer", "W", &skyWorld);
		ShaderManager::Apply();
		sky.Draw(device);

		trees.Draw();

		tRex.Draw();

		win.Present();

		if (win.inputs.KeyPressed(VK_ESCAPE))
			break;
	}

	return 0;
}