#include "CharacterController.h"
#include "Level.h"
#include "DeferredRendering.h"
#include <DirectXMath.h>

const unsigned int WIDTH  = 1280;
const unsigned int HEIGHT = 720;

// load all textures and shaders
static void LoadShadersAndTextures(DXCore* _driver)
{
	// shaders
	ShaderManager::Init(_driver);

	ShaderManager::AddVertex("Default", "Resources/Shaders/Vertex/DefaultVertex.hlsl");
	ShaderManager::AddPixel("Default", "Resources/Shaders/Pixel/DefaultPixel.hlsl");
	ShaderManager::AddPixel("Depth", "Resources/Shaders/Pixel/DepthOnlyPixel.hlsl");

	// textures
	TextureManager::Init(_driver);

	TextureManager::load("Resources/Textures/Wall.png");
	TextureManager::load("Resources/Textures/Ground.jpg");
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	CreateDirectory(L"Cache", NULL); // create a cache directory if not present to store compiled data

	Camera camera(Vec2(WIDTH, HEIGHT), Vec3(0, 2, 10), Vec3(0, 0, 0), 0.1f, 1000);
	Window win(WIDTH, HEIGHT, "GTA-TRex");
	DXCore* driver = &win.GetDevice();

	DeferredRenderer renderer;
	renderer.Init(WIDTH, HEIGHT, driver);

	LoadShadersAndTextures(driver);
	WrapSampler sampler1(*driver);
	sampler1.Bind(*driver);
	ClampSampler sampler2(*driver);
	sampler2.Bind(*driver);

	Timer timer;
	win.inputs.SetCursorLock(true);

	// different meshes-------------------------------------------------------

	 MeshData* cube     = Primitives::Cube(driver);
	 Matrix cubeWorld   = Matrix::Translation(Vec3(2, 5, 0)) * Matrix::Scaling(Vec3(1,5,1));
	 
	 MeshData* sphere   = Primitives::Sphere(10, 10, 1, driver);
	 Matrix sphereWorld = Matrix::Translation(Vec3(-2, 1, 0));
	 
	 MeshData* ground   = Primitives::Plane(driver);
	 Matrix groundWorld = Matrix::Scaling(10);

	//-------------------------------------------------------------------------

	float dt;
	float frames = 0, time = 0;

	// light data
	float ambInt[2] = { 0.3f,  4 };
	renderer.UpdateConstant("LightBuffer", "AmbInt", &ambInt);

	Vec3 lightDir      = Vec3(1, -1, 1).Normalize();
	Matrix lightProj   = Matrix::PerProject(45, (float)WIDTH/(float)HEIGHT, 0.1f, 1000);
	Matrix lightView   = Matrix::View(-lightDir * 20, lightDir);
	Matrix lightVP     = lightProj * lightView;

	Matrix cameraIProj = camera.GetProjMat().Inverse();

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
		win.Update();
		camera.Update(dt);

		// update sun light and sky
		Matrix rot = Matrix::RotateY(fmod(time * 20, 360));
		lightDir   = rot.MulPoint(Vec3(1, -1, 1));
		lightView  = Matrix::View(-lightDir * 10, lightDir);
		lightVP    = lightProj * lightView;

		// view projection matrix from camera
		Matrix vp         = camera.GetViewProjMat();
		Matrix cameraView = camera.GetViewMat();
		ShaderManager::UpdateAll(ShaderStage::Vertex, "ConstBuffer", "VP", &vp);

		renderer.clear();

		// geometru pass--------------
		renderer.SetGeometryPass();

		ShaderManager::Set("Default", "Default");
		ShaderManager::UpdatePixel("tex", TextureManager::find("Wall.png"));
		ShaderManager::UpdateVertex("ConstBuffer", "W", &cubeWorld);
		ShaderManager::Apply();
		cube->Draw(driver);
		
		ShaderManager::UpdateVertex("ConstBuffer", "W", &sphereWorld);
		ShaderManager::Apply();
		sphere->Draw(driver);
		
		ShaderManager::UpdatePixel("tex", TextureManager::find("Ground.jpg"));
		ShaderManager::UpdateVertex("ConstBuffer", "W", &groundWorld);
		ShaderManager::Apply();
		ground->Draw(driver);

		//----------------------------

		// light pass ----------------
		ShaderManager::UpdateAll(ShaderStage::Vertex, "ConstBuffer", "VP", &lightVP);
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

		// Deffered shading part
		Matrix cameraIView = cameraView.Inverse();
		renderer.UpdateConstant("ConstBuffer", "IV", &cameraIView);
		renderer.UpdateConstant("ConstBuffer", "IP", &cameraIProj);

		renderer.UpdateConstant("LightBuffer", "LV", &lightView);
		renderer.UpdateConstant("LightBuffer", "LP", &lightProj);

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

	return 0;
}