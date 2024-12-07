#pragma once
#include "Level.h"


void Trees::Init(DXCore* _driver)
{
	driver = _driver;
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

void Trees::Update(float _dt)
{
	time += _dt;
}

void Trees::Draw()
{
	ShaderManager::UpdateConstant("Leaf", ShaderStage::VertexShader, "ConstBuffer", "T", &time);

	int mesh = 0, total = meshes.size();
	for (Matrix& worldMat : worldMats)
	{
		ShaderManager::UpdateConstant("Tree", ShaderStage::VertexShader, "ConstBuffer", "W", &worldMat);
		ShaderManager::UpdateConstant("Leaf", ShaderStage::VertexShader, "ConstBuffer", "W", &worldMat);

		int index = mesh++ % meshes.size();
		for (int i = 0; i < meshes[index].meshes.size(); i++)
		{
			// select shader
			if (meshes[index].textureFiles[i].find("branch") == std::string::npos)
				ShaderManager::Set("Tree");
			else
				ShaderManager::Set("Leaf");

			ShaderManager::UpdateTexture(ShaderStage::PixelShader, "tex", TextureManager::find(meshes[index].textureFiles[i])); //update texture
			ShaderManager::UpdateTexture(ShaderStage::PixelShader, "nor", TextureManager::find(meshes[index].normalFiles[i])); //update normal
			ShaderManager::Apply();

			//draw meshes with same texture
			for (auto& meshData : meshes[index].meshes[i])
				meshData.Draw(driver);
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

void CustomMesh::Init(Vec3 _pos, DXCore* _driver)
{
	driver = _driver;

	transform.scale = Vec3(0.05f);
	transform.position = _pos;
	transform.Update();

	mesh.Init("Resources/Trees/Models/banana1.gem", driver);

	size = Vec3(4);
	offset.y += 1;
	mass = 0.2f;
	Collisions::AddCollider(this);
}

void CustomMesh::Draw()
{

	ShaderManager::Set("Default");
	ShaderManager::UpdateConstant(ShaderStage::VertexShader, "ConstBuffer", "W", &transform.worldMat);
	ShaderManager::UpdateTexture(ShaderStage::PixelShader, "tex", TextureManager::find("Wall.png"));
	//ShaderManager::UpdateTexture(ShaderStage::PixelShader, "nor", TextureManager::find("Wall_normal.png"));
	ShaderManager::Apply();
	mesh.Draw(driver);
}

CustomMesh::~CustomMesh()
{
	Collisions::RemoveCollider(this);
}



Level::Level(DXCore* _driver)
{
	driver = _driver;

	//trees.Init(driver);
	//
	//box.Init(Vec3(0, 2, -10), driver);
	//
	//object.Init(Vec3(0, 2, 10), driver);
	//object.isStatic = true;

	sky = Sphere(50, 50, 250, driver);
	skyWorld = Matrix::RotateX(180);
}

void Level::Update(float _dt)
{
	//trees.Update(_dt);
}

void Level::Draw()
{
	ShaderManager::Set("Default");
	ShaderManager::UpdateConstant(ShaderStage::VertexShader, "ConstBuffer", "W", &skyWorld);
	ShaderManager::UpdateTexture(ShaderStage::PixelShader, "tex", TextureManager::find("Sky.jpg"));
	ShaderManager::Apply();
	sky.Draw(driver);

	ground.Draw();
	//trees.Draw();
	//box.Draw();
	//object.Draw();
}