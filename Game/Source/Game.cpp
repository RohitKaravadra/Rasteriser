#include "CharacterController.h"
#include "Level.h"
#include "FullScreenQuad.h"
#include "RenderTarget.h"

const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 1080;

// load all textures and shaders
static void LoadShadersAndTextures(DXCore* _driver)
{
	// shaders
	ShaderManager::Init(_driver);
	ShaderManager::Add("Gizmos", "Resources/Shaders/Vertex/DefaultVertex.hlsl", "Resources/Shaders/Pixel/GizmosPixel.hlsl");
	ShaderManager::Add("Default", "Resources/Shaders/Vertex/DefaultVertex.hlsl", "Resources/Shaders/Pixel/DefaultPixel.hlsl");
	ShaderManager::Add("NormalMap", "Resources/Shaders/Vertex/DefaultVertex.hlsl", "Resources/Shaders/Pixel/NormalMapPixel.hlsl");
	ShaderManager::Add("DefaultTiling", "Resources/Shaders/Vertex/DefaultVertex.hlsl", "Resources/Shaders/Pixel/DefaultTilingPixel.hlsl");
	ShaderManager::Add("Tree", "Resources/Shaders/Vertex/InstancingVertex.hlsl", "Resources/Shaders/Pixel/NormalMapPixel.hlsl", ShaderType::Instancing);
	ShaderManager::Add("Leaf", "Resources/Shaders/Vertex/AnimatedVertex.hlsl", "Resources/Shaders/Pixel/NormalMapPixel.hlsl", ShaderType::Instancing);
	ShaderManager::Add("TRex", "Resources/Shaders/Vertex/BoneAnimatedVertex.hlsl", "Resources/Shaders/Pixel/NormalMapPixel.hlsl", ShaderType::Animated);

	// textures
	TextureManager::Init(_driver);
	TextureManager::load("bark07.png", "Resources/Trees/Textures/bark07.png");
	TextureManager::load("bark07_Normal.png", "Resources/Trees/Textures/bark07_Normal.png");
	TextureManager::load("fir branch.png", "Resources/Trees/Textures/fir branch.png");
	TextureManager::load("fir branch_Normal.png", "Resources/Trees/Textures/fir branch_Normal.png");

	TextureManager::load("bark09.png", "Resources/Trees/Textures/bark09.png");
	TextureManager::load("bark09_Normal.png", "Resources/Trees/Textures/bark09_Normal.png");
	TextureManager::load("stump01.png", "Resources/Trees/Textures/stump01.png");
	TextureManager::load("stump01_Normal.png", "Resources/Trees/Textures/stump01_Normal.png");
	TextureManager::load("pine branch.png", "Resources/Trees/Textures/pine branch.png");
	TextureManager::load("pine branch_Normal.png", "Resources/Trees/Textures/pine branch_Normal.png");

	TextureManager::load("T-rex_Base_Color.png", "Resources/TRex/Textures/T-rex_Base_Color.png");
	TextureManager::load("T-rex_Normal_OpenGl.png", "Resources/TRex/Textures/T-rex_Normal_OpenGl.png");

	TextureManager::load("Sky.jpg", "Resources/Textures/Sky.jpg");
	TextureManager::load("Ground.jpg", "Resources/Textures/Ground.jpg");

	TextureManager::load("Wall.png", "Resources/Textures/Wall.png");
	TextureManager::load("Wall_normal.png", "Resources/Textures/Wall_normal.png");
}

//int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
int main()
{
	Camera camera(Vec2(WIDTH, HEIGHT), Vec3(0, 5, 10), Vec3(0, 0, 0), 0.1f, 1000.f);
	Window win(WIDTH, HEIGHT, "My Window", false, 100, 50);
	DXCore* driver = &win.GetDevice();

	FullScreenQuad screenQuad("Resources/Shaders/Vertex/FullScreenQuadVertex.hlsl", "Resources/Shaders/Pixel/FullScreenQuadPixel.hlsl", driver);

	Collisions::Init(driver);

	LoadShadersAndTextures(driver);
	Sampler sampler(*driver);
	sampler.Bind(*driver);

	Timer timer;
	win.inputs.SetCursorLock(true);

	Level level(driver);
	CharacterController character(Vec3(0, 5, 0), Vec3::zero, Vec3::one);

	RenderTarget rdt(WIDTH, HEIGHT, driver);

	float dt;

	bool freeLook = false;
	while (true)
	{
		// refresh inputs
		win.Update();
		Collisions::Update();

		dt = timer.dt();

		// free look camera update
		if (freeLook)
			camera.Update(dt);

		// character controller updates camera
		if (!freeLook)
			character.Update(dt);
		// update trees
		level.Update(dt);

		// view projection matrix from camera
		Matrix vp = camera.GetViewProjMat();
		ShaderManager::UpdateConstantForAll(ShaderStage::VertexShader, "ConstBuffer", "VP", &vp);

		driver->Clear();
		rdt.Clear(driver);
		rdt.Apply(driver);

		level.Draw();
		if (!freeLook)
			character.Draw();

		Collisions::DrawGizmos();

		driver->ApplyBackbufferView();

		screenQuad.UpdateTexture("tex", rdt.srv, driver);
		screenQuad.Draw(driver);

		driver->Present();

		if (win.inputs.ButtonDown(2) || win.inputs.KeyDown(VK_ESCAPE))
			win.inputs.ToggleCursorLock();

		if (win.inputs.Exit())
			break;
	}

	ShaderManager::Free();
	TextureManager::Free();

	return 0;
}