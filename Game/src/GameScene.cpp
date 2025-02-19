#include "../hdr/CharacterController.h"
#include "../hdr/Level.h"
#include "../../Engine/hdr/DeferredRendering.h"
#include "../../Engine//hdr/Gizmos.h"

const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;

// load all textures and shaders
static void LoadShadersAndTextures(DXCore* _driver)
{
	// shaders
	ShaderManager::Init(_driver);

	ShaderManager::AddVertex("Default", "Resources/Shaders/vs_Default.hlsl");
	ShaderManager::AddVertex("Grass", "Resources/Shaders/vs_Grass.hlsl", LayoutType::Instancing);
	ShaderManager::AddVertex("Leaves", "Resources/Shaders/vs_Leaves.hlsl", LayoutType::Instancing);
	ShaderManager::AddVertex("TreeLeaf", "Resources/Shaders/vs_Animated.hlsl", LayoutType::Instancing);
	ShaderManager::AddVertex("Tree", "Resources/Shaders/vs_Instancing.hlsl", LayoutType::Instancing);
	ShaderManager::AddVertex("BoneAnimated", "Resources/Shaders/vs_BoneAnimated.hlsl", LayoutType::Animated);

	ShaderManager::AddPixel("Default", "Resources/Shaders/ps_Default.hlsl");
	ShaderManager::AddPixel("Tiling", "Resources/Shaders/ps_Tiling.hlsl");
	ShaderManager::AddPixel("Gizmos", "Resources/Shaders/ps_Gizmos.hlsl");
	ShaderManager::AddPixel("Normal", "Resources/Shaders/ps_NormalMap.hlsl");
	ShaderManager::AddPixel("Depth", "Resources/Shaders/ps_DepthOnly.hlsl");

	// textures
	TextureManager::Init(_driver);

	TextureManager::load("Resources/Textures/P_Purple.png");
	TextureManager::load("Resources/Textures/P_Green.png");

	TextureManager::load("Resources/Textures/Wall_normal.png");

	TextureManager::load("Resources/Textures/Grass.png");
	TextureManager::load("Resources/Textures/Leaf.png");

	TextureManager::load("Resources/Textures/Ground.png");
	TextureManager::load("Resources/Textures/Sky.png");
}

void GameScene()
{
	Camera camera(Vec2(WIDTH, HEIGHT), Vec3(0, 2, 10), Vec3(0, 0, 0), 0.1f, 1000);
	Window win(WIDTH, HEIGHT, "GTA-TRex");
	DXCore* driver = &win.GetDevice();
	Collisions::Init(driver);

	DeferredRenderer renderer;
	renderer.Init(WIDTH, HEIGHT, driver);

	LoadShadersAndTextures(driver);

	Gizmos::Init(driver);

	WrapSampler sampler1(*driver);
	ClampSampler sampler2(*driver);
	sampler1.Bind(*driver);
	sampler2.Bind(*driver);

	Timer timer;
	win.inputs.SetCursorLock(true);


	// different meshes-------------------------------------------------------

	Level* level = new Level(driver);
	Ground* ground = new Ground(driver);
	Sky* sky = new Sky(driver);
	CharacterController* controller = new CharacterController(Vec3(0, 2, 0), Vec3(0), Vec3(1));
	//-------------------------------------------------------------------------

	float dt;
	float frames = 0, time = 0;

	// light data
	float ambInt[2] = { 0.3f,  2 };
	renderer.UpdateConstant("LightBuffer", "AmbInt", &ambInt);

	float litDist = 300;
	Vec3 litDir = Vec3(-1, -1, 0).Normalize();

	Matrix litView = Matrix::View(-litDir * litDist, litDir);
	Matrix litProj = Matrix::OrthoProject(WIDTH / 4, HEIGHT / 4, 0.1f, 1000.f);
	//Matrix litProj = Matrix::PerProject(45.f,(float) WIDTH/ HEIGHT , 0.1f, 1000.f);
	Matrix litVP = litProj * litView;

	Matrix camProj = camera.GetProjMat();
	Matrix camIProj = camProj.Inverse();

	timer.reset();
	while (true)
	{
		dt = timer.dt();
		Collisions::Update();

		// limit too large delta time to avoid unexpected behaviour
		if (dt > 2)
			dt = 0.00001f;

		// lock fps of game to max 120
		if (1 / dt > 120)
			continue;

		timer.reset();

		// update inputs
		win.Update();

		controller->Update(dt);
		//camera.Update(dt);
		level->Update(dt);

		// update sun light and sky
		float angle = fmod(time * 2, 360);
		Vec3 newLitDir = Matrix::RotateY(angle).MulPoint(litDir);

		litView = Matrix::View(-newLitDir * litDist, newLitDir);
		litVP = litProj * litView;

		sky->Rotate(angle);

		// view projection matrix from camera
		Matrix vp = camera.GetViewProjMat();
		Matrix cameraView = camera.GetViewMat();

		// testing shadows
		//Matrix vp         = litVP;
		//Matrix cameraView = litView;

		renderer.Clear();

		// light pass ----------------
		ShaderManager::UpdateAll(ShaderStage::Vertex, "ConstBuffer", "VP", &litVP);
		renderer.SetLightPass();

		//ShaderManager::Set("Default", "Depth");
		//ShaderManager::lockPixel = true;

		controller->Draw();
		level->Draw();

		//ShaderManager::lockPixel = false;
		//----------------------------

		// geometru pass--------------
		ShaderManager::UpdateAll(ShaderStage::Vertex, "ConstBuffer", "VP", &vp);
		renderer.SetGeometryPass();

		controller->Draw();
		level->Draw();
		ground->Draw();
		sky->Draw();

		//----------------------------

		// Draw Gizmos----------------

		Gizmos::Set();
		Gizmos::Draw(Gizmo::Sphere, Matrix::Translation(-newLitDir * litDist) * Matrix::Scaling(5));
		//Gizmos::Reset();
		Collisions::DrawGizmos();
		//----------------------------

		// Deffered shading part -----
		Matrix cameraIView = cameraView.Inverse();

		renderer.UpdateConstant("ConstBuffer", "IV", &cameraIView);
		renderer.UpdateConstant("ConstBuffer", "IP", &camIProj);

		renderer.UpdateConstant("LightBuffer", "LV", &litView);
		renderer.UpdateConstant("LightBuffer", "LP", &litProj);

		renderer.Draw();
		//----------------------------

		driver->Present();

		if (win.inputs.ButtonDown(2) || win.inputs.KeyDown(VK_ESCAPE))
			win.inputs.ToggleCursorLock();

		if (win.inputs.Exit())
			break;

		// frame evaluation 
		frames++;
		time += dt;
	}

	ShaderManager::Free();
	TextureManager::Free();

	delete level;
	delete ground;
	delete sky;

	//std::string avgFps = "Average Fps : " + std::to_string(frames / time);
	//MessageBoxA(NULL, avgFps.c_str(), "Evaluation ", 0);
}