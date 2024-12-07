#include "Collisions.h"
#include "Shader.h"
#include <algorithm>

Collider::Collider()
{
	isStatic = false;
	mass = 1;
	isEnabled = true;
}

bool Collider::Collide(Collider& _other)
{
	// calculate positions from offsets
	Vec3 myPos = transform.position + offset;
	Vec3 otPos = _other.transform.position + _other.offset;

	// calculate direction and distance
	Vec3 dist = myPos - otPos;
	Vec3 delta = dist.Abs() - (size + _other.size) / 2;

	// check for collision
	if (delta.x < 0 && delta.y < 0 && delta.z < 0)
	{
		float w = mass + _other.mass;
		if (!isStatic)
			Resolve(dist, delta, _other.isStatic ? 1 : _other.mass / w);
		if (!_other.isStatic)
			_other.Resolve(-dist, delta, isStatic ? 1 : mass / w);
		return true;
	}
	return false;
}

void Collider::Resolve(Vec3 _dist, Vec3 _delta, float _push)
{
	// create resolving normal from direction = _dist and intersection = _delta
	Vec3 normal = 0;
	if (_delta.x > _delta.y)
	{
		if (_delta.x > _delta.z)
			normal.x = _dist.x < 0 ? -1 : 1;
		else
			normal.z = _dist.z < 0 ? -1 : 1;
	}
	else
	{
		if (_delta.y > _delta.z)
			normal.y = _dist.y < 0 ? -1 : 1;
		else
			normal.z = _dist.z < 0 ? -1 : 1;
	}

	// update position according to push amount
	transform.position -= _delta * normal * _push;
	transform.Update();
}

void Collider::Draw(Cube& _gizmo, DXCore* _driver)
{
	Matrix world = Matrix::World(transform.position + offset, size / 2);
	ShaderManager::UpdateConstant(ShaderStage::VertexShader, "ConstBuffer", "W", &world);
	ShaderManager::Apply();
	_gizmo.Draw(_driver);
}


std::vector<Collider*> Collisions::layer0;
std::vector<Collider*> Collisions::layer1;
Cube Collisions::cubeGizmo;
DXCore* Collisions::driver = nullptr;

void Collisions::Init(DXCore* _driver)
{
	driver = _driver;
	cubeGizmo = Cube(driver);
}

void Collisions::AddCollider(Collider* _collider, int _layer)
{
	switch (_layer)
	{
	case 1:
		if (find(layer1.begin(), layer1.end(), _collider) == layer1.end())
			layer1.push_back(_collider);
		break;
	default:
		if (find(layer0.begin(), layer0.end(), _collider) == layer0.end())
			layer0.push_back(_collider);
	}
}

void Collisions::RemoveCollider(Collider* _collider)
{
	auto at = find(layer0.begin(), layer0.end(), _collider);
	if (at == layer0.end())
	{
		at = find(layer1.begin(), layer1.end(), _collider);
		if (at == layer1.end())
			return;
		layer1.erase(at);
	}
	else
		layer0.erase(at);
}

void Collisions::Update()
{
	for (int i = 0; i < layer0.size() - 1; i++)
	{
		for (int j = i + 1; j < layer0.size(); j++)
		{
			if (!layer0[i]->isEnabled || !layer0[j]->isEnabled)
				continue;

			if (layer0[i]->Collide(*layer0[j]))
			{
				layer0[i]->OnCollision(*layer0[j]);
				layer0[j]->OnCollision(*layer0[i]);
			}
		}
	}
}

void Collisions::DrawGizmos()
{
	driver->UpdateRasterizerState(DrawType::Outline);
	ShaderManager::Set("Gizmos");
	for (Collider* col : layer0)
	{
		if (col->isEnabled)
			col->Draw(cubeGizmo, driver);
	}
	for (Collider* col : layer1)
	{
		if (col->isEnabled)
			col->Draw(cubeGizmo, driver);
	}
	driver->UpdateRasterizerState();
}