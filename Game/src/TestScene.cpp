#include "../../Engine/hdr/Texture.h"
#include "../../Engine/hdr/Window.h"
#include "../../Engine/hdr/DeferredRendering.h"
#include "../../Engine/hdr/Gizmos.h"
#include "../hdr/Camera.h"

const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;

// load all textures and shaders
static void LoadShadersAndTextures(DXCore* _driver)
{
	// shaders
	ShaderManager::Init(_driver);

	ShaderManager::AddVertex("Default", "Resources/Shaders/Vertex/DefaultVertex.hlsl");

	ShaderManager::AddPixel("Default", "Resources/Shaders/Pixel/TilingPixel.hlsl");
	ShaderManager::AddPixel("Gizmos", "Resources/Shaders/Pixel/GizmosPixel.hlsl");
	ShaderManager::AddPixel("Depth", "Resources/Shaders/Pixel/DepthOnlyPixel.hlsl");

	// textures
	TextureManager::Init(_driver);

	TextureManager::load("Resources/Textures/P_Purple.png");
	TextureManager::load("Resources/Textures/P_Green.png");

	TextureManager::load("Resources/Textures/Ground.png");
}

void TestScene()
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

	MeshData* cube = Primitives::Cube(driver);
	Matrix cubeWorld = Matrix::Translation(Vec3(2, 5, 0)) * Matrix::Scaling(Vec3(1, 5, 1));

	MeshData* sphere = Primitives::Sphere(20, 20, 1, driver);
	Matrix sphereWorld = Matrix::Translation(Vec3(-2, 1, 0));

	MeshData* ground = Primitives::Plane(driver);
	Matrix groundWorld = Matrix::Scaling(10);

	//-------------------------------------------------------------------------

	float dt;
	float frames = 0, time = 0;

	// light data
	float ambInt[2] = { 0.3f,  2 };
	renderer.UpdateConstant("LightBuffer", "AmbInt", &ambInt);

	float litDist = 100;
	Vec3 litDir = Vec3(-1, -1, 0).Normalize();

	Matrix litView = Matrix::View(-litDir * litDist, litDir);
	Matrix litProj = Matrix::OrthoProject(WIDTH / 20, HEIGHT / 20, 0.1f, 1000.f);
	Matrix litVP = litProj * litView;

	Matrix camProj = camera.GetProjMat();
	Matrix camIProj = camProj.Inverse();

	Vec3 tiling(2, 2, 2);
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

		// update sun light and sky
		float angle = fmod(time * 20, 360);
		Vec3 newLitDir = Matrix::RotateY(angle).MulPoint(litDir);

		litView = Matrix::View(-newLitDir * litDist, newLitDir);
		litVP = litProj * litView;

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

		ShaderManager::Set("Default", "Depth");
		ShaderManager::lockPixel = true;

		ShaderManager::UpdateVertex("ConstBuffer", "W", &cubeWorld);
		ShaderManager::Apply();
		cube->Draw(driver);

		ShaderManager::UpdateVertex("ConstBuffer", "W", &sphereWorld);
		ShaderManager::Apply();
		sphere->Draw(driver);

		ShaderManager::lockPixel = false;
		//----------------------------

		// geometru pass--------------
		ShaderManager::UpdateAll(ShaderStage::Vertex, "ConstBuffer", "VP", &vp);
		renderer.SetGeometryPass();

		ShaderManager::Set("Default", "Default");
		ShaderManager::UpdatePixel("ConstBuffer", "T", &tiling);
		ShaderManager::UpdatePixel("tex", TextureManager::find("P_Purple.png"));
		ShaderManager::UpdateVertex("ConstBuffer", "W", &cubeWorld);
		ShaderManager::Apply();
		cube->Draw(driver);

		ShaderManager::Set("Default", "Default");
		ShaderManager::UpdateVertex("ConstBuffer", "W", &sphereWorld);
		ShaderManager::Apply();
		sphere->Draw(driver);

		ShaderManager::UpdatePixel("tex", TextureManager::find("P_Green.png"));
		ShaderManager::UpdateVertex("ConstBuffer", "W", &groundWorld);
		ShaderManager::Apply();
		ground->Draw(driver);

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

	delete cube, ground, sphere;

	//std::string avgFps = "Average Fps : " + std::to_string(frames / time);
	//MessageBoxA(NULL, avgFps.c_str(), "Evaluation ", 0);
}