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

	ShaderManager::AddVertex("Default", "Resources/Shaders/Vertex/DefaultVertex.hlsl");
	ShaderManager::AddVertex("Grass", "Resources/Shaders/Vertex/GrassVertex.hlsl", LayoutType::Instancing);
	ShaderManager::AddVertex("Leaves", "Resources/Shaders/Vertex/LeavesVertex.hlsl", LayoutType::Instancing);
	ShaderManager::AddVertex("TreeLeaf", "Resources/Shaders/Vertex/AnimatedVertex.hlsl", LayoutType::Instancing);
	ShaderManager::AddVertex("Tree", "Resources/Shaders/Vertex/InstancingVertex.hlsl", LayoutType::Instancing);
	ShaderManager::AddVertex("BoneAnimated", "Resources/Shaders/Vertex/BoneAnimatedVertex.hlsl", LayoutType::Animated);

	ShaderManager::AddPixel("Default", "Resources/Shaders/Pixel/DefaultPixel.hlsl");
	ShaderManager::AddPixel("Tiling", "Resources/Shaders/Pixel/TilingPixel.hlsl");
	ShaderManager::AddPixel("Gizmos", "Resources/Shaders/Pixel/GizmosPixel.hlsl");
	ShaderManager::AddPixel("Normal", "Resources/Shaders/Pixel/NormalMapPixel.hlsl");
	ShaderManager::AddPixel("Depth", "Resources/Shaders/Pixel/DepthOnlyPixel.hlsl");

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
	//-------------------------------------------------------------------------

	float dt;
	float frames = 0, time = 0;

	// light data
	float ambInt[2] = { 0.3f,  2 };
	renderer.UpdateConstant("LightBuffer", "AmbInt", &ambInt);

	float litDist = 300;
	Vec3 litDir = Vec3(-1, -1, 0).Normalize();

	Matrix litView = Matrix::View(-litDir * litDist, litDir);
	Matrix litProj = Matrix::OrthoProject(WIDTH/4, HEIGHT/4, 0.1f, 1000.f);
	Matrix litVP = litProj * litView;

	Matrix camProj = camera.GetProjMat();
	Matrix camIProj = camProj.Inverse();

	while (true)
	{
		dt = timer.dt();

		// limit too large delta time to avoid unexpected behaviour
		if (dt > 2)
			dt = 0.00001f;

		// lock fps of game to max 120
		if (1 / dt > 120)
			continue;

		timer.reset();

		// update inputs
		win.Update();

		camera.Update(dt);
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

		level->Draw();

		//ShaderManager::lockPixel = false;
		//----------------------------

		// geometru pass--------------
		ShaderManager::UpdateAll(ShaderStage::Vertex, "ConstBuffer", "VP", &vp);
		renderer.SetGeometryPass();

		level->Draw();
		ground->Draw();
		sky->Draw();

		//----------------------------

		// Draw Gizmos----------------

		Gizmos::Set();
		Gizmos::Draw(Gizmo::Sphere, -newLitDir * litDist);
		Gizmos::Reset();
		//----------------------------

		// Deffered shading part
		Matrix cameraIView = cameraView.Inverse();

		renderer.UpdateConstant("ConstBuffer", "IV", &cameraIView);
		renderer.UpdateConstant("ConstBuffer", "IP", &camIProj);

		renderer.UpdateConstant("LightBuffer", "LV", &litView);
		renderer.UpdateConstant("LightBuffer", "LP", &litProj);

		renderer.Draw();

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

	delete level, ground, sky;

	//std::string avgFps = "Average Fps : " + std::to_string(frames / time);
	//MessageBoxA(NULL, avgFps.c_str(), "Evaluation ", 0);
}