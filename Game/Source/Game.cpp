#include "CharacterController.h"
#include <random>

const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 1080;

// creating tress using instancing
class Trees
{
	std::vector<Mesh> meshes;
	std::vector<Matrix> worldMats;
	DXCore* driver;
public:
	Trees(DXCore* _driver) : driver(_driver)
	{
		Mesh tree1;
		tree1.Init("Resources/Trees/Models/pine1.gem", driver);
		meshes.push_back(tree1);
		Mesh tree2;
		tree2.Init("Resources/Trees/Models/pine2.gem", driver);
		meshes.push_back(tree2);

		std::random_device rd;
		std::mt19937 rGen(rd());
		std::uniform_int_distribution<> posDistr(-400, 400);
		std::uniform_real_distribution<> scalDistr(0.04f, 0.08f);

		for (unsigned int i = 0; i < 100; i++)
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
			ShaderManager::UpdateConstant("Tree", ShaderStage::VertexShader, "ConstBuffer", "W", &worldMat);
			ShaderManager::UpdateConstant("Leaf", ShaderStage::VertexShader, "ConstBuffer", "W", &worldMat);
			int index = mesh++ % meshes.size();
			for (auto& _data : meshes[index].data)
			{
				// select shader
				if (_data.first.find("branch") == std::string::npos)
					ShaderManager::Set("Tree");
				else
					ShaderManager::Set("Leaf");

				ShaderManager::UpdateTexture(ShaderStage::PixelShader, "tex", TextureManager::find(_data.first)); //update texture
				ShaderManager::Apply();

				//draw meshes with same texture
				for (auto& meshData : _data.second)
					meshData.Draw(driver);
			}
		}
	}
};

static void LoadShadersAndTextures(DXCore* _driver)
{
	// shaders
	ShaderManager::Init(_driver);
	ShaderManager::Add("Default", "Resources/Shaders/DefaultVertex.hlsl", "Resources/Shaders/DefaultPixel.hlsl");
	ShaderManager::Add("Tree", "Resources/Shaders/TreeVertex.hlsl", "Resources/Shaders/TreePixel.hlsl");
	ShaderManager::Add("Leaf", "Resources/Shaders/AnimatedVertex.hlsl", "Resources/Shaders/TreePixel.hlsl");
	ShaderManager::Add("TRex", "Resources/Shaders/BoneAnimatedVertex.hlsl", "Resources/Shaders/DefaultPixel.hlsl", ShaderType::Animated);

	// textures
	TextureManager::Init(_driver);
	TextureManager::load("bark07.png", "Resources/Trees/Textures/bark07.png");
	TextureManager::load("fir branch.png", "Resources/Trees/Textures/fir branch.png");

	TextureManager::load("bark09.png", "Resources/Trees/Textures/bark09.png");
	TextureManager::load("stump01.png", "Resources/Trees/Textures/stump01.png");
	TextureManager::load("pine branch.png", "Resources/Trees/Textures/pine branch.png");

	TextureManager::load("T-rex_Base_Color.png", "Resources/TRex/Textures/T-rex_Base_Color.png");

	TextureManager::load("Sky.jpg", "Resources/Textures/Sky.jpg");
	TextureManager::load("Ground.jpg", "Resources/Textures/Ground.jpg");
}

//int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
int main()
{
	Camera camera(Vec2(WIDTH, HEIGHT), Vec3(0, 5, 10), Vec3(0, 0, 0), 0.1f, 1000.f);
	Window win(WIDTH, HEIGHT, "My Window", false, 100, 50);
	DXCore* driver = &win.GetDevice();

	LoadShadersAndTextures(driver);

	Timer timer;

	win.inputs.SetCursorLock(true);

	Sphere sky(50, 50, 250, driver);
	Matrix skyWorld = Matrix::RotateX(180);

	Plane plane(driver);
	Vec2 tiling;
	Matrix planeWorld = Matrix::Scaling(Vec3(50));

	Trees trees(driver);
	CharacterController character(Vec3::zero, Vec3::zero, Vec3::one);

	Sampler sampler(*driver);
	sampler.Bind(*driver);

	float dt, time = 0;

	while (true)
	{
		// refresh inputs
		win.Update();

		dt = timer.dt();
		time += dt;

		// free look camera update
		//camera.Update(dt);

		// character controller updates camera
		character.Update(dt);

		// view projection matrix from camera
		Matrix vp = camera.GetViewProjMat();

		win.Clear();

		ShaderManager::UpdateConstant("Tree", ShaderStage::VertexShader, "ConstBuffer", "VP", &vp);
		ShaderManager::UpdateConstant("Leaf", ShaderStage::VertexShader, "ConstBuffer", "VP", &vp);
		ShaderManager::UpdateConstant("Leaf", ShaderStage::VertexShader, "ConstBuffer", "T", &time);
		trees.Draw();

		ShaderManager::Set("Default");
		ShaderManager::UpdateConstant(ShaderStage::VertexShader, "ConstBuffer", "VP", &vp);
		ShaderManager::UpdateTexture(ShaderStage::PixelShader, "tex", TextureManager::find("Ground.jpg"));
		ShaderManager::UpdateConstant(ShaderStage::VertexShader, "ConstBuffer", "W", &planeWorld);
		ShaderManager::Apply();
		plane.Draw(driver);

		ShaderManager::UpdateConstant(ShaderStage::VertexShader, "ConstBuffer", "VP", &vp);
		ShaderManager::UpdateTexture(ShaderStage::PixelShader, "tex", TextureManager::find("Sky.jpg"));
		ShaderManager::UpdateConstant(ShaderStage::VertexShader, "ConstBuffer", "W", &skyWorld);
		ShaderManager::Apply();
		sky.Draw(driver);

		character.Draw();

		win.Present();

		if (win.inputs.ButtonDown(2) || win.inputs.KeyDown(VK_ESCAPE))
			win.inputs.ToggleCursorLock();

		if (win.inputs.Exit())
			break;
	}

	ShaderManager::Free();
	TextureManager::Free();

	return 0;
}