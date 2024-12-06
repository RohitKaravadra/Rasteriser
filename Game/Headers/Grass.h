#pragma once
#include "Mesh.h"
#include "Camera.h"
#include <random>

class Grass
{
	std::vector<Matrix> worldMats;
	Vec3 pos;

public:
	Grass(Vec3 _pos, Vec3 _area, unsigned int _amount)
	{
		std::random_device rd;
		std::mt19937 rGen(rd());
		std::uniform_int_distribution<> posDistr(-400, 400);
		std::uniform_real_distribution<> scalDistr(0.04f, 0.08f);

		for (unsigned int i = 0; i < 500; i++)
		{
			Matrix mat = Matrix::Translation(Vec3(posDistr(rGen), 0, posDistr(rGen))) * Matrix::Scaling(scalDistr(rGen));
			worldMats.push_back(mat);
		}
	}
};