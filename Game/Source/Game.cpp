#include "CharacterController.h"
#include "Level.h"

const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;

// load all textures and shaders
static void LoadShadersAndTextures(DXCore* _driver)
{
	// shaders
	ShaderManager::Init(_driver);
	ShaderManager::Add("Gizmos", "Resources/Shaders/Vertex/DefaultVertex.hlsl", "Resources/Shaders/Pixel/GizmosPixel.hlsl");
	ShaderManager::Add("Default", "Resources/Shaders/Vertex/DefaultVertex.hlsl", "Resources/Shaders/Pixel/DefaultPixel.hlsl");
	//ShaderManager::Add("NormalMap", "Resources/Shaders/Vertex/DefaultVertex.hlsl", "Resources/Shaders/Pixel/NormalMapPixel.hlsl");
	//ShaderManager::Add("DefaultTiling", "Resources/Shaders/Vertex/DefaultVertex.hlsl", "Resources/Shaders/Pixel/DefaultTilingPixel.hlsl");

	//ShaderManager::Add("Grass", "Resources/Shaders/Vertex/GrassVertex.hlsl", "Resources/Shaders/Pixel/DefaultPixel.hlsl", ShaderType::Instancing);
	//ShaderManager::Add("Leaves", "Resources/Shaders/Vertex/LeavesVertex.hlsl", "Resources/Shaders/Pixel/DefaultPixel.hlsl", ShaderType::Instancing);
	//ShaderManager::Add("Tree", "Resources/Shaders/Vertex/InstancingVertex.hlsl", "Resources/Shaders/Pixel/NormalMapPixel.hlsl", ShaderType::Instancing);
	//ShaderManager::Add("Leaf", "Resources/Shaders/Vertex/AnimatedVertex.hlsl", "Resources/Shaders/Pixel/NormalMapPixel.hlsl", ShaderType::Instancing);

	//ShaderManager::Add("TRex", "Resources/Shaders/Vertex/BoneAnimatedVertex.hlsl", "Resources/Shaders/Pixel/NormalMapPixel.hlsl", ShaderType::Animated);

	// textures
	TextureManager::Init(_driver);

	//TextureManager::load("Resources/Textures/Wall.png");
	//TextureManager::load("Resources/Textures/Wall_normal.png");

	//TextureManager::load("Resources/Textures/Grass.png");
	//TextureManager::load("Resources/Textures/Leaf.png");

	TextureManager::load("Resources/Textures/Ground.jpg");
	TextureManager::load("Resources/Textures/Sky.jpg");
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	// create a cache directory if not present to store compiled data
	CreateDirectory(L"Cache", NULL);

	Camera camera(Vec2(WIDTH, HEIGHT), Vec3(0, 5, 10), Vec3(0, 0, 0), 0.1f, 1000.f);
	Window win(WIDTH, HEIGHT, "GTA-TRex");
	DXCore* driver = &win.GetDevice();

	FullScreenQuad screenQuad("Resources/Shaders/Vertex/FullScreenQuadVertex.hlsl", "Resources/Shaders/Pixel/FullScreenQuadPixel.hlsl", driver);
	RenderTarget rdt(WIDTH, HEIGHT, driver);

	Collisions::Init(driver);

	LoadShadersAndTextures(driver);
	Sampler sampler(*driver);
	sampler.Bind(*driver);


	Level level(driver);
	// CharacterController character(Vec3(0, 5, 0), Vec3::zero, Vec3::one);

	// settings inputs
	bool freeLook = true;//MessageBoxA(NULL, "Free Look?", "Mode", MB_YESNO) == IDYES ? true : false;
	bool debug = false;//MessageBoxA(NULL, "Debug?", "Sub Mode", MB_YESNO) == IDYES ? true : false;

	Timer timer;
	win.inputs.SetCursorLock(true);

	float dt;
	float frames = 0, time = 0;

	// set light ambient value
	float ambient = 0.2f, intensity = 2;
	ShaderManager::UpdateConstantForAll(ShaderStage::PixelShader, "ConstBuffer", "Amb", &ambient);
	ShaderManager::UpdateConstantForAll(ShaderStage::PixelShader, "ConstBuffer", "Int", &intensity);

	while (true)
	{
		dt = timer.dt();

		// limit too large delta time to avoid unexpected behaviour
		if (dt > 2)
			dt = 0.00001f;

		// lock fps of game to max 120
		if (1 / dt <= 120)
			timer.reset();
		else
			continue;

		// refresh inputs
		win.Update();

		// free look camera update
		if (freeLook)
			camera.Update(dt);

		// character controller updates camera
		//if (!freeLook)
			//character.Update(dt);

		// update trees
		level.Update(dt);

		// update collisions if any
		// make sure to update after all the objects in scene are updated
		Collisions::Update();

		// view projection matrix from camera
		Matrix vp = camera.GetViewProjMat();
		ShaderManager::UpdateConstantForAll(ShaderStage::VertexShader, "ConstBuffer", "VP", &vp);

		driver->Clear();
		rdt.Clear(driver);
		rdt.Apply(driver);

		level.Draw();
		//if (!freeLook)
			//character.Draw();

		if (debug)
			Collisions::DrawGizmos();

		// Deffered shading part
		driver->ApplyBackbufferView();
		screenQuad.DrawTexture("tex", rdt.srv, driver);
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
	Collisions::Free();

	//std::string avgFps = "Average Fps : " + std::to_string(frames / time);
	//MessageBoxA(NULL, avgFps.c_str(), "Evaluation ", 0);
}