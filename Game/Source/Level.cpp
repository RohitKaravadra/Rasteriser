#pragma once
#include "Level.h"

void Particles::Init(Vec3 _volume, Vec3 _pos, unsigned int _total, DXCore* _driver)
{
	driver = _driver;
	_pos -= _volume / 2;

	MeshData* billboard = Primitives::BillBoard(_driver);
	mesh.Copy(*billboard);
	delete billboard;

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
}

void Particles::Draw()
{
	ShaderManager::Set("Leaves", "Default");
	ShaderManager::UpdateVertex("ConstBuffer", "T", &time);
	ShaderManager::UpdateVertex("ConstBuffer", "D", &height);
	ShaderManager::UpdatePixel("tex", TextureManager::find("Leaf.png"));
	ShaderManager::Apply();
	mesh.Draw(driver);
}

void Grass::Init(Vec2 _area, unsigned int _total, DXCore* _driver)
{
	driver = _driver;

	MeshData* billboard = Primitives::BillBoard(_driver, 2);
	mesh.Copy(*billboard);
	delete billboard;

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
}

void Grass::Draw()
{
	ShaderManager::Set("Grass", "Default");
	ShaderManager::UpdateVertex("ConstBuffer", "T", &time);
	ShaderManager::UpdatePixel("tex", TextureManager::find("Grass.png"));
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
	ShaderManager::UpdateVertex("TreeLeaf", "ConstBuffer", "T", &time);
	ShaderManager::UpdateVertex("TreeLeaf", "ConstBuffer", "W", &worldMat);

	ShaderManager::UpdateVertex("Tree", "ConstBuffer", "W", &worldMat);

	ShaderManager::SetPixel("Normal");
	for (InstancedMesh& tree : meshes)
	{
		for (int i = 0; i < tree.meshes.size(); i++)
		{
			ShaderManager::SetVertex(tree.textureFiles[i].find("branch") == std::string::npos ? "Tree" : "TreeLeaf");

			ShaderManager::UpdatePixel("tex", TextureManager::find(tree.textureFiles[i])); //update texture
			ShaderManager::UpdatePixel("nor", TextureManager::find(tree.normalFiles[i])); //update normal
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
	plane = Primitives::Plane(driver);

	size = Vec3(900, 1, 900);
	offset = Vec3::down * 0.5f;
	isStatic = true;
	tag = "Ground";
	Collisions::AddCollider(this);
}

void Ground::Draw()
{

	ShaderManager::Set("Default", "Tiling");
	ShaderManager::UpdateVertex("ConstBuffer", "W", &transform.worldMat);
	ShaderManager::UpdatePixel("ConstBuffer", "T", &tiling);
	ShaderManager::UpdatePixel("tex", TextureManager::find("Ground.jpg"));
	ShaderManager::Apply();
	plane->Draw(driver);
}

Ground::~Ground()
{
	Collisions::RemoveCollider(this);
	delete plane;
}


void Box::Init(Vec3 _pos, DXCore* _driver)
{
	driver = _driver;

	transform.scale = Vec3(2);
	transform.position = _pos;
	transform.Update();

	box = Primitives::Cube(driver);

	size = Vec3(4);
	mass = 0.6f;

	Collisions::AddCollider(this);
}

void Box::Draw()
{

	ShaderManager::Set("Defautl", "Normal");
	ShaderManager::UpdateVertex("ConstBuffer", "W", &transform.worldMat);
	ShaderManager::UpdatePixel("tex", TextureManager::find("Wall.png"));
	ShaderManager::UpdatePixel("nor", TextureManager::find("Wall_normal.png"));
	ShaderManager::Apply();
	box->Draw(driver);
}

Box::~Box()
{
	Collisions::RemoveCollider(this);
	delete box;
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
}

void Level::Update(float _dt)
{
	time += _dt;

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
}