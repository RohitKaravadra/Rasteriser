#pragma once
#include "Level.h"

void Particles::Init(Vec3 _volume, Vec3 _pos, unsigned int _total, DXCore* _driver)
{
	driver = _driver;
	_pos -= _volume / 2;

	Billboard billboard;
	billboard.Init(driver);
	mesh.Copy(billboard.mesh);

	std::random_device rd;
	std::mt19937 rGen(rd());
	std::uniform_real_distribution<> xDistr(0, _volume.x);
	std::uniform_real_distribution<> yDistr(0, _volume.y);
	std::uniform_real_distribution<> zDistr(0, _volume.z);

	for (int i = 0; i < _total; i++)
		positions.push_back(_pos + Vec3(xDistr(rd), yDistr(rd), zDistr(rd)));

	mesh.SetInstanceData(sizeof(Vec3), positions.size(), &positions[0], driver);
	camera = Camera::GetInstance();
}

void Particles::Update(float _dt)
{
	time += _dt;
	worldMat = Matrix::LookAt(Vec3::zero, camera->transform.position) * Matrix::Scaling(0.7f);
}

void Grass::Init(Vec2 _area, unsigned int _total, DXCore* _driver)
{
	driver = _driver;

	worldMat = Matrix::Scaling(Vec3(5));
	Billboard billboard;
	billboard.Init(driver);
	mesh.Copy(billboard.mesh);
	Vec3 pos(-_area.x / 2, 0, -_area.y / 2);

	std::random_device rd;
	std::mt19937 rGen(rd());
	std::uniform_real_distribution<> xDistr(0, _area.x);
	std::uniform_real_distribution<> yDistr(0, _area.y);

	for (int i = 0; i < _total; i++)
		positions.push_back(pos + Vec3(xDistr(rd), 0, yDistr(rd)));

	mesh.SetInstanceData(sizeof(Vec3), positions.size(), &positions[0], driver);
	camera = Camera::GetInstance();
}

void Grass::Update(float _dt)
{
	time += _dt;
	worldMat = Matrix::LookAt(Vec3::zero, camera->transform.position) * Matrix::Scaling(Vec3(3));
}

void Grass::Draw()
{
	ShaderManager::Set("Grass");
	ShaderManager::UpdateConstant(ShaderStage::VertexShader, "ConstBuffer", "T", &time);
	ShaderManager::UpdateConstant(ShaderStage::VertexShader, "ConstBuffer", "W", &worldMat);
	ShaderManager::UpdateTexture(ShaderStage::PixelShader, "tex", TextureManager::find("Grass.png"));
	ShaderManager::Apply();
	mesh.Draw(driver);
}

void Particles::Draw()
{
	ShaderManager::Set("Leaves");
	ShaderManager::UpdateConstant(ShaderStage::VertexShader, "ConstBuffer", "T", &time);
	ShaderManager::UpdateConstant(ShaderStage::VertexShader, "ConstBuffer", "D", &height);
	ShaderManager::UpdateConstant(ShaderStage::VertexShader, "ConstBuffer", "W", &worldMat);
	ShaderManager::UpdateTexture(ShaderStage::PixelShader, "tex", TextureManager::find("Leaf.png"));
	ShaderManager::Apply();
	mesh.Draw(driver);
}

void Trees::Init(unsigned int _total, DXCore* _driver)
{
	driver = _driver;

	worldMat = Matrix::Scaling(Vec3(0.05f));

	// load tree data if available
	std::string filePath = "Resources/Trees.txt";
	std::ifstream file(filePath);
	if (file)
	{
		try
		{
			std::string line;
			getline(file, line);
			int total = std::stoi(line);

			for (int i = 0; i < total; i++)
			{
				std::string location;
				std::string textLocation;

				getline(file, location);
				getline(file, textLocation);

				InstancedMesh tree;
				tree.Init(location, driver, textLocation);
				meshes.push_back(tree);

				std::vector<Vec3> treepos;
				positions.push_back(treepos);
			}
			file.close();
		}
		catch (const std::exception& e)
		{
			std::string msg = "File Format incorrect for Trees.txt\nDelete file to see default format";
			MessageBoxA(NULL, msg.c_str(), "ERROR", 0);
			file.close();
			exit(0);
		}
	}
	else
	{
		// save default data if file is not present
		std::ofstream newfile(filePath);
		newfile << "2\n";
		newfile << "Resources/Trees/Models/pine1.gem\nResources/Trees/Textures/\n";
		newfile << "Resources/Trees/Models/pine4.gem\nResources/Trees/Textures/\n";
		newfile.close();

		// create default trees
		InstancedMesh tree1;
		InstancedMesh tree2;

		tree1.Init("Resources/Trees/Models/pine1.gem", driver, "Resources/Trees/Textures/");
		tree2.Init("Resources/Trees/Models/pine4.gem", driver, "Resources/Trees/Textures/");

		meshes.push_back(tree1);
		meshes.push_back(tree2);

		std::vector<Vec3> tree1pos;
		std::vector<Vec3> tree2pos;

		positions.push_back(tree1pos);
		positions.push_back(tree2pos);
	}

	std::random_device rd;
	std::mt19937 rGen(rd());
	std::uniform_real_distribution<> posDistr(-400, 400);
	std::uniform_int_distribution<> treeDistr(0, meshes.size() - 1);

	for (unsigned int i = 0; i < _total; i++)
	{
		Vec3 pos = Vec3(posDistr(rGen), 0, posDistr(rGen));
		int treeIndex = treeDistr(rGen);
		positions[treeIndex].push_back(pos);
	}

	for (int i = 0; i < meshes.size(); i++)
		meshes[i].SetInstanceData(sizeof(Vec3), positions[i].size(), &positions[i][0], driver);
}

void Trees::Update(float _dt)
{
	time += _dt;
}

void Trees::Draw()
{
	ShaderManager::UpdateConstant("Leaf", ShaderStage::VertexShader, "ConstBuffer", "T", &time);

	ShaderManager::UpdateConstant("Tree", ShaderStage::VertexShader, "ConstBuffer", "W", &worldMat);
	ShaderManager::UpdateConstant("Leaf", ShaderStage::VertexShader, "ConstBuffer", "W", &worldMat);

	for (InstancedMesh& tree : meshes)
	{
		for (int i = 0; i < tree.meshes.size(); i++)
		{
			if (tree.textureFiles[i].find("branch") == std::string::npos)
				ShaderManager::Set("Tree");
			else
				ShaderManager::Set("Leaf");

			ShaderManager::UpdateTexture(ShaderStage::PixelShader, "tex", TextureManager::find(tree.textureFiles[i])); //update texture
			ShaderManager::UpdateTexture(ShaderStage::PixelShader, "nor", TextureManager::find(tree.normalFiles[i])); //update normal
			ShaderManager::Apply();

			for (InstancedMeshData& mesh : tree.meshes[i])
				mesh.Draw(driver);
		}
	}
}

Ground::Ground()
{
	transform.scale = Vec3(90, 1, 90);
	transform.position = Vec3(0, 0, 0);
	transform.Update();

	driver = &Window::GetInstance()->GetDevice();
	tiling = Vec2(20, 20);
	plane = Plane(driver);

	size = Vec3(900, 1, 900);
	offset = Vec3::down * 0.5f;
	isStatic = true;
	tag = "Ground";
	Collisions::AddCollider(this);
}

void Ground::Draw()
{

	ShaderManager::Set("DefaultTiling");
	ShaderManager::UpdateConstant(ShaderStage::VertexShader, "ConstBuffer", "W", &transform.worldMat);
	ShaderManager::UpdateConstant(ShaderStage::PixelShader, "ConstBuffer", "T", &tiling);
	ShaderManager::UpdateTexture(ShaderStage::PixelShader, "tex", TextureManager::find("Ground.jpg"));
	ShaderManager::Apply();
	plane.Draw(driver);
}

Ground::~Ground()
{
	Collisions::RemoveCollider(this);
}


void Box::Init(Vec3 _pos, DXCore* _driver)
{
	driver = _driver;

	transform.scale = Vec3(2);
	transform.position = _pos;
	transform.Update();

	box = Cube(driver);

	size = Vec3(4);
	mass = 0.2f;

	Collisions::AddCollider(this);
}

void Box::Draw()
{

	ShaderManager::Set("NormalMap");
	ShaderManager::UpdateConstant(ShaderStage::VertexShader, "ConstBuffer", "W", &transform.worldMat);
	ShaderManager::UpdateTexture(ShaderStage::PixelShader, "tex", TextureManager::find("Wall.png"));
	ShaderManager::UpdateTexture(ShaderStage::PixelShader, "nor", TextureManager::find("Wall_normal.png"));
	ShaderManager::Apply();
	box.Draw(driver);
}

Box::~Box()
{
	Collisions::RemoveCollider(this);
}

Level::Level(DXCore* _driver)
{
	driver = _driver;

	trees.Init(200, driver);
	grass.Init(Vec2(500, 500), 300, driver);
	particles.Init(Vec3(500, 20, 500), Vec3(0, 12, 0), 1000, driver);

	box.Init(Vec3(0, 2, -10), driver);
	staticObject.Init(Vec3(0, 2, 10), driver);
	staticObject.isStatic = true;

	sky = Sphere(50, 50, 250, driver);
	skyWorld;
}

void Level::Update(float _dt)
{
	time += _dt;

	// update light and sky box
	Matrix rot = Matrix::RotateY(fmod(time, 360));
	skyWorld = rot * Matrix::RotateX(180);
	Vec3 lightDir = rot.MulPoint(Vec3(-2, 1, -1)).Normalize();
	ShaderManager::UpdateConstantForAll(ShaderStage::PixelShader, "ConstBuffer", "Dir", &lightDir);

	// update trees , grass and particles for vertex animation
	trees.Update(_dt);
	grass.Update(_dt);
	particles.Update(_dt);
}

void Level::Draw()
{
	box.Draw();
	staticObject.Draw();

	trees.Draw();
	grass.Draw();
	particles.Draw();

	ground.Draw();

	ShaderManager::Set("Default");
	ShaderManager::UpdateConstant(ShaderStage::VertexShader, "ConstBuffer", "W", &skyWorld);
	ShaderManager::UpdateTexture(ShaderStage::PixelShader, "tex", TextureManager::find("Sky.jpg"));
	ShaderManager::Apply();
	sky.Draw(driver);
}