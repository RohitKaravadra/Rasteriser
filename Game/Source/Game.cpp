#include "CharacterController.h"
#include <random>

const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 1080;

Matrix vp;

// creating tress using instancing
class Trees
{
	std::vector<Mesh> meshes;
	std::vector<Matrix> worldMats;
	DXCore* driver;

	float time;
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

	void Update(float _dt)
	{
		time += _dt;
	}

	void Draw()
	{
		ShaderManager::UpdateConstant("Leaf", ShaderStage::VertexShader, "ConstBuffer", "T", &time);

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
	ShaderManager::Add("Gizmos", "Resources/Shaders/Vertex/DefaultVertex.hlsl", "Resources/Shaders/Pixel/GizmosPixel.hlsl");
	ShaderManager::Add("Default", "Resources/Shaders/Vertex/DefaultVertex.hlsl", "Resources/Shaders/Pixel/DefaultPixel.hlsl");
	ShaderManager::Add("DefaultTiling", "Resources/Shaders/Vertex/DefaultVertex.hlsl", "Resources/Shaders/Pixel/DefaultTilingPixel.hlsl");
	ShaderManager::Add("Tree", "Resources/Shaders/Vertex/TreeVertex.hlsl", "Resources/Shaders/Pixel/TreePixel.hlsl");
	ShaderManager::Add("Leaf", "Resources/Shaders/Vertex/AnimatedVertex.hlsl", "Resources/Shaders/Pixel/TreePixel.hlsl");
	ShaderManager::Add("TRex", "Resources/Shaders/Vertex/BoneAnimatedVertex.hlsl", "Resources/Shaders/Pixel/DefaultPixel.hlsl", ShaderType::Animated);

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

class Ground :public Collider
{
	Plane plane;
	Vec2 tiling;

	DXCore* driver;
public:
	Ground()
	{
		transform.scale = Vec3(50, 1, 50);
		transform.position = Vec3(0, 0, 0);
		transform.Update();

		driver = &Window::GetInstance()->GetDevice();
		tiling = Vec2(20, 20);
		plane = Plane(driver);

		size = Vec3(500, 1, 500);
		offset = Vec3::down * 0.5f;
		isStatic = true;
		enableGizmo = true;
		Collisions::AddCollider(this);
	}

	void Draw()
	{

		ShaderManager::Set("DefaultTiling");
		ShaderManager::UpdateConstant(ShaderStage::VertexShader, "ConstBuffer", "W", &transform.worldMat);
		ShaderManager::UpdateConstant(ShaderStage::PixelShader, "ConstBuffer", "T", &tiling);
		ShaderManager::UpdateTexture(ShaderStage::PixelShader, "tex", TextureManager::find("Ground.jpg"));
		ShaderManager::Apply();
		plane.Draw(driver);
	}

	~Ground()
	{
		Collisions::RemoveCollider(this);
	}
};

//int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
int main()
{
	Camera camera(Vec2(WIDTH, HEIGHT), Vec3(0, 5, 10), Vec3(0, 0, 0), 0.1f, 1000.f);
	Window win(WIDTH, HEIGHT, "My Window", false, 100, 50);
	DXCore* driver = &win.GetDevice();
	Collisions::Init(driver);

	LoadShadersAndTextures(driver);

	Timer timer;

	win.inputs.SetCursorLock(true);

	Sphere sky(50, 50, 250, driver);
	Matrix skyWorld = Matrix::RotateX(180);

	Trees trees(driver);
	CharacterController character(Vec3::zero, Vec3::zero, Vec3::one);

	Ground ground;

	Sampler sampler(*driver);
	sampler.Bind(*driver);

	float dt;

	while (true)
	{
		// refresh inputs
		win.Update();
		Collisions::Update();

		dt = timer.dt();

		// free look camera update
		//camera.Update(dt);

		// character controller updates camera
		character.Update(dt);
		// update trees
		trees.Update(dt);

		// view projection matrix from camera
		vp = camera.GetViewProjMat();
		ShaderManager::UpdateConstantForAll(ShaderStage::VertexShader, "ConstBuffer", "VP", &vp);

		win.Clear();

		ground.Draw();
		trees.Draw();

		character.Draw();

		ShaderManager::Set("Default");
		ShaderManager::UpdateConstant(ShaderStage::VertexShader, "ConstBuffer", "W", &skyWorld);
		ShaderManager::UpdateTexture(ShaderStage::PixelShader, "tex", TextureManager::find("Sky.jpg"));
		ShaderManager::Apply();
		sky.Draw(driver);

		Collisions::DrawGizmos();

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