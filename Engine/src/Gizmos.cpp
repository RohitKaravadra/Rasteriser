#include "../hdr/Gizmos.h"
#include "../hdr/Shader.h"

DXCore* Gizmos::driver = nullptr;
MeshData Gizmos::shapes[2];

void Gizmos::Init(DXCore* _driver) 
{ 
	driver = _driver; 

	MeshData* mesh = Primitives::Sphere(5,5,1, driver);
	shapes[0].Copy(*mesh);
	delete mesh;

	mesh = Primitives::Cube(driver);
	shapes[1].Copy(*mesh);
	delete mesh;
}

void Gizmos::Set()
{
	driver->SetRasterizerState(DrawType::Outline);
	ShaderManager::Set("Default", "Gizmos");
}

void Gizmos::Draw(Gizmo _type, Matrix _mat)
{
	ShaderManager::UpdateVertex("ConstBuffer", "W", &_mat);
	ShaderManager::Apply();
	shapes[_type].Draw(driver);
}

void Gizmos::Draw(Gizmo _type, Vec3 _pos)
{
	Matrix mat = Matrix::Translation(_pos);
	ShaderManager::UpdateVertex("ConstBuffer", "W", &mat);
	ShaderManager::Apply();
	shapes[_type].Draw(driver);
}

void Gizmos::Reset()
{
	driver->SetRasterizerState(DrawType::Solid);
	ShaderManager::Set("Default", "Default");
}