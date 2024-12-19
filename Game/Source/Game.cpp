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
	ShaderManager::Add("Gizmos", "Resources/Shaders/Vertex/DefaultVertex.hlsl", "Resources/Shaders/Pixel/GizmosPixel.hlsl");
	ShaderManager::Add("Default", "Resources/Shaders/Vertex/DefaultVertex.hlsl", "Resources/Shaders/Pixel/DefaultPixel.hlsl");
	ShaderManager::Add("NormalMap", "Resources/Shaders/Vertex/DefaultVertex.hlsl", "Resources/Shaders/Pixel/NormalMapPixel.hlsl");
	ShaderManager::Add("DefaultTiling", "Resources/Shaders/Vertex/DefaultVertex.hlsl", "Resources/Shaders/Pixel/DefaultTilingPixel.hlsl");

	ShaderManager::Add("Grass", "Resources/Shaders/Vertex/GrassVertex.hlsl", "Resources/Shaders/Pixel/DefaultPixel.hlsl", ShaderType::Instancing);
	ShaderManager::Add("Leaves", "Resources/Shaders/Vertex/LeavesVertex.hlsl", "Resources/Shaders/Pixel/DefaultPixel.hlsl", ShaderType::Instancing);
	ShaderManager::Add("Tree", "Resources/Shaders/Vertex/InstancingVertex.hlsl", "Resources/Shaders/Pixel/NormalMapPixel.hlsl", ShaderType::Instancing);
	ShaderManager::Add("Leaf", "Resources/Shaders/Vertex/AnimatedVertex.hlsl", "Resources/Shaders/Pixel/NormalMapPixel.hlsl", ShaderType::Instancing);

	ShaderManager::Add("TRex", "Resources/Shaders/Vertex/BoneAnimatedVertex.hlsl", "Resources/Shaders/Pixel/NormalMapPixel.hlsl", ShaderType::Animated);

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
//int main()
{
	// create a cache directory if not present to store compiled data
	CreateDirectory(L"Cache", NULL);

	Camera camera(Vec2(WIDTH, HEIGHT), Vec3(0, 5, 10), Vec3(0, 0, 0), 0.1f, 1000.f);
	Window win(WIDTH, HEIGHT, "GTA-TRex");
	DXCore* driver = &win.GetDevice();

	DeferredRenderer renderer;
	renderer.Init(WIDTH, HEIGHT, driver);

	Collisions::Init(driver);

	LoadShadersAndTextures(driver);
	Sampler sampler(*driver);
	sampler.Bind(*driver);


	Level level(driver);
	MeshData* sky = Primitives::Sphere(50, 50, 250, driver);
	Matrix skyWVP;
	CharacterController character(Vec3(0, 5, 0), Vec3::zero, Vec3::one);

	// settings inputs
	bool freeLook = false;//MessageBoxA(NULL, "Free Look?", "Mode", MB_YESNO) == IDYES ? true : false;
	bool debug = true;//MessageBoxA(NULL, "Debug?", "Sub Mode", MB_YESNO) == IDYES ? true : false;

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
			character.Update(dt);

		// update trees
		level.Update(dt);

		// update sun light and sky
		Matrix rot = Matrix::RotateY(fmod(time, 360));
		Vec3 lightDir = rot.MulPoint(Vec3(-1, 1, -1));
		skyWVP = rot * Matrix::RotateX(180);

		renderer.UpdateConstant("ConstBuffer", "Dir", &lightDir);

		// update collisions if any
		// make sure to update after all the objects in scene are updated
		Collisions::Update();

		// view projection matrix from camera
		Matrix vp = camera.GetViewProjMat();
		ShaderManager::UpdateConstantForAll(ShaderStage::VertexShader, "ConstBuffer", "VP", &vp);

		renderer.SetPassOne();

		if (!freeLook)
			character.Draw();

		level.Draw();

		if (debug)
			Collisions::DrawGizmos();

		// draw sky
		ShaderManager::Set("Default");
		ShaderManager::UpdateConstant(ShaderStage::VertexShader, "ConstBuffer", "W", &skyWVP);
		ShaderManager::UpdateTexture(ShaderStage::PixelShader, "tex", TextureManager::find("Sky.jpg"));
		ShaderManager::Apply();
		sky->Draw(driver);

		// Deffered shading part
		renderer.Draw();

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

	//std::string avgFps = "Average Fps : " + std::to_string(frames / time);
	//MessageBoxA(NULL, avgFps.c_str(), "Evaluation ", 0);

	//return 0;
}