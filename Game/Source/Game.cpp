#include "CharacterController.h"
#include "Level.h"
#include "DeferredRendering.h"

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

	// textures
	TextureManager::Init(_driver);

	TextureManager::load("Resources/Textures/Wall.png");
	TextureManager::load("Resources/Textures/Wall_normal.png");

	TextureManager::load("Resources/Textures/Grass.png");
	TextureManager::load("Resources/Textures/Leaf.png");

	TextureManager::load("Resources/Textures/Ground.jpg");
	TextureManager::load("Resources/Textures/Sky.jpg");
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	CreateDirectory(L"Cache", NULL); // create a cache directory if not present to store compiled data

	// initialize camera, window and driver
	Camera camera(Vec2(WIDTH, HEIGHT), Vec3(0, 2, 0), Vec3(0, 0, 0), 0.1f, 1000);
	Window win(WIDTH, HEIGHT, "GTA-TRex");
	DXCore* driver = &win.GetDevice();

	// create deferred renderer
	DeferredRenderer renderer;
	renderer.Init(WIDTH, HEIGHT, driver);

	// initalize collision system
	Collisions::Init(driver);

	// load shaders and textures
	LoadShadersAndTextures(driver);
	Sampler sampler(*driver);	// create a sampler
	sampler.Bind(*driver);		// bind the sampler

	// create sky sphere
	MeshData* sky = Primitives::Sphere(50, 50, 250, driver);
	Matrix4x4 skyWVP;

	// create level
	Level* level = new Level(driver);

	// create character controller
	CharacterController* character = new CharacterController(Vec3(0, 5, 0), Vec3::zero, Vec3::one);

	// settings inputs
	bool freeLook = MessageBoxA(NULL, "Free Look?", "Mode", MB_YESNO) == IDYES ? true : false;
	bool debug = MessageBoxA(NULL, "Debug?", "Sub Mode", MB_YESNO) == IDYES ? true : false;

	Timer timer;
	win.inputs.SetCursorLock(true);

	float dt;
	float frames = 0, time = 0;

	// set light ambient value
	float ambient = 0.3f, intensity = 4;
	renderer.UpdateConstant("ConstBuffer", "Amb", &ambient);
	renderer.UpdateConstant("ConstBuffer", "Int", &intensity);

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
		if (!freeLook)
			character->Update(dt);

		// update trees
		level->Update(dt);

		// update sun light and sky
		Matrix4x4 rot = Matrix4x4::RotateY(fmod(time * 2, 360));
		Vec3 lightDir = rot.MulPoint(Vec3(1, -1, 1));
		skyWVP = rot * Matrix4x4::RotateX(180);

		renderer.UpdateConstant("ConstBuffer", "Dir", &lightDir);

		// update collisions if any
		// make sure to update after all the objects in scene are updated
		Collisions::Update();

		// view projection matrix from camera
		Matrix4x4 vp = camera.GetViewProjMat();
		ShaderManager::UpdateAll(ShaderStage::Vertex, "ConstBuffer", "VP", &vp);

		renderer.GeometryPass();

		character->Draw();
		level->Draw();

		if (debug)
			Collisions::DrawGizmos();

		// draw sky
		ShaderManager::Set("Default", "Default");
		ShaderManager::UpdateVertex("ConstBuffer", "W", &skyWVP);
		ShaderManager::UpdatePixel("tex", TextureManager::find("Sky.jpg"));
		ShaderManager::Apply();
		sky->Draw(driver);

		// Deffered shading part
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
	Collisions::Free();
	delete sky;
	delete level;
	delete character;

	// std::string avgFps = "Average Fps : " + std::to_string(frames / time);
	// MessageBoxA(NULL, avgFps.c_str(), "Evaluation ", 0);

	return 0;
}