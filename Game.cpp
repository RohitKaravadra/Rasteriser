#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include <sstream>
#include <random>
#include "Texture.h"

const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 1080;

// creating tress using instancing
class Trees
{
	std::vector<StaticMesh> meshes;
	std::vector<Matrix> worldMats;
	DXCore& driver;
public:
	Trees(unsigned int _total, DXCore& _driver) : driver(_driver)
	{
		StaticMesh tree1;
		tree1.Init("Models/Trees/Models/pine1.gem", driver);
		meshes.push_back(tree1);
		StaticMesh tree2;
		tree2.Init("Models/Trees/Models/beech.gem", driver);
		meshes.push_back(tree2);
		StaticMesh tree3;
		tree3.Init("Models/Trees/Models/oak.gem", driver);
		meshes.push_back(tree3);
		StaticMesh tree4;
		tree4.Init("Models/Trees/Models/birch.gem", driver);
		meshes.push_back(tree4);

		std::random_device rd;
		std::mt19937 rGen(rd());
		std::uniform_int_distribution<> posDistr(-400, 400);
		std::uniform_real_distribution<> scalDistr(0.01f, 0.05f);

		for (unsigned int i = 0; i < _total; i++)
		{
			Matrix mat = Matrix::Translation(Vec3(posDistr(rGen), 0, posDistr(rGen))) * Matrix::Scaling(scalDistr(rGen));
			worldMats.push_back(mat);
		}
	}
	void Draw()
	{
		int mesh = 0, total = meshes.size();
		for (Matrix& worldMat : worldMats)
		{
			ShaderManager::UpdateConstant(ShaderStage::VertexShader, "staticMeshBuffer", "W", &worldMat);
			ShaderManager::Apply();
			meshes[mesh++ % total].Draw(driver);
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
		ShaderManager::UpdateConstant(ShaderStage::VertexShader, "animatedMeshBuffer", "W", &transform.worldMat);
		ShaderManager::UpdateConstant(ShaderStage::VertexShader, "animatedMeshBuffer", "bones", &instance.matrices);
		ShaderManager::Apply();
		mesh.Draw(driver);
	}
};

class Tree :public StaticBehaviour
{
	StaticMesh mesh;
	DXCore& driver;
public:
	Tree(Vec3 _pos, Vec3 _rot, Vec3 _scale, DXCore& _driver) : driver(_driver), StaticBehaviour(_pos, _rot, _scale)
	{
		mesh.Init("Models/Pine/pine.gem", driver);
	}

	void Draw() override
	{
		ShaderManager::Set("Tree");
		ShaderManager::UpdateConstant(ShaderStage::VertexShader, "staticMeshBuffer", "W", &transform.worldMat);
		ShaderManager::Apply();
		mesh.Draw(driver);
	}
};

//int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
int main()
{
	Window win;
	Timer timer;
	TextureManager textureManager;
	Sampler sampler;

	Camera camera(Vec2(WIDTH, HEIGHT), Vec3(0, 5, 10), Vec3(0, 0, 0));

	win.Create(WIDTH, HEIGHT, "My Window", false, 100, 100);
	DXCore& driver = win.GetDevice();
	win.inputs.SetCursorLock(true);

	Sphere sky(50, 50, 250, driver);
	Matrix skyWorld;

	Plane plane(driver);
	Matrix planeWorld = Matrix::Scaling(Vec3(50));

	Trees trees(200, driver);
	TRex tRex(Vec3::zero, Vec3::zero, Vec3::one, driver);

	ShaderManager::Init(driver);
	ShaderManager::Add("Tree", "Shaders/TreeVertexShader.hlsl", "Shaders/TreePixelShader.hlsl");
	ShaderManager::Add("Plane", "Shaders/StaticMeshVertexShader.hlsl", "Shaders/StaticMeshPixelShader.hlsl");
	ShaderManager::Add("TRex", "Shaders/AnimatedMeshVertexShader.hlsl", "Shaders/TreePixelShader.hlsl", true);

	textureManager.load("Models/Pine/Textures/pine branch.png", driver);
	textureManager.load("Models/TRex/Textures/T-rex_Base_Color.png", driver);
	textureManager.load("Textures/Sky.jpg", driver);
	textureManager.load("Textures/Ground.jpg", driver);

	sampler.Init(driver);
	sampler.Bind(driver);

	float dt, moveSpeed, rotSpeed = 50;

	while (true)
	{
		// refresh inputs
		win.Update();

		dt = timer.dt();

		// camera movement
		if (win.inputs.KeyPressed('F'))
			camera.Position(Vec3(1));

		Vec2 moveDelta = win.inputs.GetAxis() * dt;
		Vec2 rotDelta = win.inputs.GetCursorLock() ? -win.inputs.MouseDelta() * dt : Vec2(0, 0);

		moveSpeed = win.inputs.KeyPressed(VK_SHIFT) ? 100 : 10;

		if (moveDelta.Length() > 0)
			camera.TranslateRel(Vec3(moveDelta.x, 0, moveDelta.y) * moveSpeed);
		if (rotDelta.Length() > 0)
			camera.Rotate(Vec3(rotDelta.y, rotDelta.x, 0) * rotSpeed);

		// view projection matrix from camera
		Matrix vp = camera.GetViewProjMat();
		tRex.Update(dt);
		
		win.Clear();

		ShaderManager::Set("Tree");
		ShaderManager::UpdateConstant(ShaderStage::VertexShader, "staticMeshBuffer", "VP", &vp);
		ShaderManager::UpdateTexture("tex", textureManager.find("Textures/Sky.jpg"));
		ShaderManager::UpdateConstant(ShaderStage::VertexShader, "staticMeshBuffer", "W", &skyWorld);
		ShaderManager::Apply();
		sky.Draw(driver);

		ShaderManager::UpdateTexture("tex", textureManager.find("Textures/Ground.jpg"));
		ShaderManager::UpdateConstant(ShaderStage::VertexShader, "staticMeshBuffer", "W", &planeWorld);
		ShaderManager::Apply();
		plane.Draw(driver);

		ShaderManager::UpdateTexture("tex", textureManager.find("Models/Pine/Textures/pine branch.png"));
		trees.Draw();

		ShaderManager::Set("TRex");
		ShaderManager::UpdateConstant(ShaderStage::VertexShader, "animatedMeshBuffer", "VP", &vp);
		ShaderManager::UpdateTexture("tex", textureManager.find("Models/TRex/Textures/T-rex_Base_Color.png"));
		tRex.Draw();


		win.Present();

		if (win.inputs.ButtonDown(2) || win.inputs.KeyDown(VK_ESCAPE))
			win.inputs.ToggleCursorLock();
	}

	ShaderManager::Free();
	return 0;
}