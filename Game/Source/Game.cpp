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
	Trees(unsigned int _total, DXCore* _driver) : driver(_driver)
	{
		Mesh tree1;
		tree1.Init("Models/Pine/pine.gem", driver);
		meshes.push_back(tree1);
		/*StaticMesh tree2;
		tree2.Init("Models/Trees/Models/beech.gem", driver);
		meshes.push_back(tree2);
		StaticMesh tree3;
		tree3.Init("Models/Trees/Models/oak.gem", driver);
		meshes.push_back(tree3);
		StaticMesh tree4;
		tree4.Init("Models/Trees/Models/birch.gem", driver);
		meshes.push_back(tree4);*/

		std::random_device rd;
		std::mt19937 rGen(rd());
		std::uniform_int_distribution<> posDistr(-400, 400);
		std::uniform_real_distribution<> scalDistr(0.01f, 0.05f);

		for (unsigned int i = 0; i < _total; i++)
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
			int index = mesh++ % meshes.size();
			ShaderManager::UpdateConstant(ShaderStage::VertexShader, "ConstBuffer", "W", &worldMat);
			for (auto& _data : meshes[index].data)
			{
				//update texture
				ShaderManager::UpdateTexture(ShaderStage::PixelShader, "tex", TextureManager::find(_data.first));
				ShaderManager::Apply();

				//draw meshes with same texture
				for (auto& meshData : _data.second)
					meshData.Draw(driver);
			}
		}
	}
};

void LoadShadersAndTextures(DXCore* _driver)
{
	ShaderManager::Init(_driver);
	ShaderManager::Add("Tree", "Shaders/TreeVertexShader.hlsl", "Shaders/TreePixelShader.hlsl");
	ShaderManager::Add("Plane", "Shaders/StaticMeshVertexShader.hlsl", "Shaders/StaticMeshPixelShader.hlsl");
	ShaderManager::Add("TRex", "Shaders/AnimatedMeshVertexShader.hlsl", "Shaders/TreePixelShader.hlsl", true);

	TextureManager::Init(_driver);
	TextureManager::load("bark09.png", "Models/Pine/Textures/bark09.png");
	TextureManager::load("pine branch.png", "Models/Pine/Textures/pine branch.png");
	TextureManager::load("stump01.png", "Models/Pine/Textures/stump01.png");
	TextureManager::load("T-rex_Base_Color.png", "Models/TRex/Textures/T-rex_Base_Color.png");
	TextureManager::load("Sky.jpg", "Textures/Sky.jpg");
	TextureManager::load("Ground.jpg", "Textures/Ground.jpg");
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
	Matrix skyWorld;

	Plane plane(driver);
	Matrix planeWorld = Matrix::Scaling(Vec3(50));

	Trees trees(200, driver);
	CharacterController character(Vec3(0), Vec3(0), Vec3(1));

	Sampler sampler(*driver);
	sampler.Bind(*driver);

	float dt;

	while (true)
	{
		// refresh inputs
		win.Update();

		dt = timer.dt();

		// character controller updates camera
		character.Update(dt);

		// view projection matrix from camera
		Matrix vp = camera.GetViewProjMat();
		skyWorld = Matrix::Translation(camera.GetPos());

		win.Clear();

		ShaderManager::Set("Tree");
		ShaderManager::UpdateConstant(ShaderStage::VertexShader, "ConstBuffer", "VP", &vp);
		ShaderManager::UpdateTexture(ShaderStage::PixelShader, "tex", TextureManager::find("Sky.jpg"));
		ShaderManager::UpdateConstant(ShaderStage::VertexShader, "ConstBuffer", "W", &skyWorld);
		ShaderManager::Apply();
		sky.Draw(driver);

		ShaderManager::UpdateConstant(ShaderStage::VertexShader, "ConstBuffer", "VP", &vp);
		ShaderManager::UpdateTexture(ShaderStage::PixelShader, "tex", TextureManager::find("Ground.jpg"));
		ShaderManager::UpdateConstant(ShaderStage::VertexShader, "ConstBuffer", "W", &planeWorld);
		ShaderManager::Apply();
		plane.Draw(driver);

		trees.Draw();
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