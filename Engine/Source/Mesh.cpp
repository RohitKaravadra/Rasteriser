#include "Mesh.h"
#include "GEMLoader.h"

static STATIC_VERTEX addVertex(Vec3 p, Vec3 n, float tu, float tv)
{
	STATIC_VERTEX v;
	v.pos = p;
	v.normal = n;
	v.tangent = Vec3(0, 0, 0); // For now
	v.tu = tu;
	v.tv = tv;
	return v;
}



void Mesh::Init(std::vector<STATIC_VERTEX> vertices, std::vector<unsigned int> indices, DXCore& _driver)
{
	Init(&vertices[0], sizeof(STATIC_VERTEX), vertices.size(), &indices[0], indices.size(), _driver);
}

void Mesh::Draw(DXCore& _driver)
{
	UINT offsets = 0;
	_driver.devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_driver.devicecontext->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);
	_driver.devicecontext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	_driver.devicecontext->DrawIndexed(indicesSize, 0, 0);
}

Plane::Plane(DXCore& _driver)
{
	std::vector<STATIC_VERTEX> vertices;
	vertices.push_back(addVertex(Vec3(-15, 0, -15), Vec3(0, 1, 0), 0, 0));
	vertices.push_back(addVertex(Vec3(15, 0, -15), Vec3(0, 1, 0), 1, 0));
	vertices.push_back(addVertex(Vec3(-15, 0, 15), Vec3(0, 1, 0), 0, 1));
	vertices.push_back(addVertex(Vec3(15, 0, 15), Vec3(0, 1, 0), 1, 1));
	std::vector<unsigned int> indices;
	indices.push_back(2); indices.push_back(1); indices.push_back(0);
	indices.push_back(1); indices.push_back(2); indices.push_back(3);

	mesh.Init(vertices, indices, _driver);
}

void Plane::Draw(DXCore& _driver) { mesh.Draw(_driver); }

Cube::Cube(DXCore& _driver)
{
	std::vector<STATIC_VERTEX> vertices;
	Vec3 p0 = Vec3(-1.0f, -1.0f, -1.0f);
	Vec3 p1 = Vec3(1.0f, -1.0f, -1.0f);
	Vec3 p2 = Vec3(1.0f, 1.0f, -1.0f);
	Vec3 p3 = Vec3(-1.0f, 1.0f, -1.0f);
	Vec3 p4 = Vec3(-1.0f, -1.0f, 1.0f);
	Vec3 p5 = Vec3(1.0f, -1.0f, 1.0f);
	Vec3 p6 = Vec3(1.0f, 1.0f, 1.0f);
	Vec3 p7 = Vec3(-1.0f, 1.0f, 1.0f);

	// front
	vertices.push_back(addVertex(p0, Vec3(0.0f, 0.0f, -1.0f), 0.0f, 1.0f));
	vertices.push_back(addVertex(p1, Vec3(0.0f, 0.0f, -1.0f), 1.0f, 1.0f));
	vertices.push_back(addVertex(p2, Vec3(0.0f, 0.0f, -1.0f), 1.0f, 0.0f));
	vertices.push_back(addVertex(p3, Vec3(0.0f, 0.0f, -1.0f), 0.0f, 0.0f));
	// back
	vertices.push_back(addVertex(p5, Vec3(0.0f, 0.0f, 1.0f), 0.0f, 1.0f));
	vertices.push_back(addVertex(p4, Vec3(0.0f, 0.0f, 1.0f), 1.0f, 1.0f));
	vertices.push_back(addVertex(p7, Vec3(0.0f, 0.0f, 1.0f), 1.0f, 0.0f));
	vertices.push_back(addVertex(p6, Vec3(0.0f, 0.0f, 1.0f), 0.0f, 0.0f));
	// left
	vertices.push_back(addVertex(p4, Vec3(-1.0f, 0.0f, 0.0f), 0.0f, 1.0f));
	vertices.push_back(addVertex(p0, Vec3(-1.0f, 0.0f, 0.0f), 1.0f, 1.0f));
	vertices.push_back(addVertex(p3, Vec3(-1.0f, 0.0f, 0.0f), 1.0f, 0.0f));
	vertices.push_back(addVertex(p7, Vec3(-1.0f, 0.0f, 0.0f), 0.0f, 0.0f));

	// right
	vertices.push_back(addVertex(p1, Vec3(1.0f, 0.0f, 0.0f), 0.0f, 1.0f));
	vertices.push_back(addVertex(p5, Vec3(1.0f, 0.0f, 0.0f), 1.0f, 1.0f));
	vertices.push_back(addVertex(p6, Vec3(1.0f, 0.0f, 0.0f), 1.0f, 0.0f));
	vertices.push_back(addVertex(p2, Vec3(1.0f, 0.0f, 0.0f), 0.0f, 0.0f));
	// top
	vertices.push_back(addVertex(p3, Vec3(0.0f, 1.0f, 0.0f), 0.0f, 1.0f));
	vertices.push_back(addVertex(p2, Vec3(0.0f, 1.0f, 0.0f), 1.0f, 1.0f));
	vertices.push_back(addVertex(p6, Vec3(0.0f, 1.0f, 0.0f), 1.0f, 0.0f));
	vertices.push_back(addVertex(p7, Vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f));
	// bottom
	vertices.push_back(addVertex(p4, Vec3(0.0f, -1.0f, 0.0f), 0.0f, 1.0f));
	vertices.push_back(addVertex(p5, Vec3(0.0f, -1.0f, 0.0f), 1.0f, 1.0f));
	vertices.push_back(addVertex(p1, Vec3(0.0f, -1.0f, 0.0f), 1.0f, 0.0f));
	vertices.push_back(addVertex(p0, Vec3(0.0f, -1.0f, 0.0f), 0.0f, 0.0f));


	std::vector<unsigned int> indices;
	indices.push_back(0); indices.push_back(1); indices.push_back(2);
	indices.push_back(0); indices.push_back(2); indices.push_back(3);
	indices.push_back(4); indices.push_back(5); indices.push_back(6);
	indices.push_back(4); indices.push_back(6); indices.push_back(7);
	indices.push_back(8); indices.push_back(9); indices.push_back(10);
	indices.push_back(8); indices.push_back(10); indices.push_back(11);
	indices.push_back(12); indices.push_back(13); indices.push_back(14);
	indices.push_back(12); indices.push_back(14); indices.push_back(15);
	indices.push_back(16); indices.push_back(17); indices.push_back(18);
	indices.push_back(16); indices.push_back(18); indices.push_back(19);
	indices.push_back(20); indices.push_back(21); indices.push_back(22);
	indices.push_back(20); indices.push_back(22); indices.push_back(23);

	mesh.Init(vertices, indices, _driver);
}

void Cube::Draw(DXCore& _driver) { mesh.Draw(_driver); }

Sphere::Sphere(unsigned int rings, unsigned int segments, unsigned int radius, DXCore& _driver)
{
	std::vector<STATIC_VERTEX> vertices;
	for (int lat = 0; lat <= rings; lat++) {
		float theta = lat * PI / rings;
		float sinTheta = sinf(theta);
		float cosTheta = cosf(theta);
		for (int lon = 0; lon <= segments; lon++) {
			float phi = lon * PI2 / segments;
			float sinPhi = sinf(phi);
			float cosPhi = cosf(phi);
			Vec3 position(radius * sinTheta * cosPhi, radius * cosTheta, radius * sinTheta * sinPhi);
			Vec3 normal = position.Normalize();
			float tu = 1.0f - (float)lon / segments;
			float tv = 1.0f - (float)lat / rings;
			vertices.push_back(addVertex(position, normal, tu, tv));
		}
	}

	std::vector<unsigned int> indices;
	for (int lat = 0; lat < rings; lat++)
	{
		for (int lon = 0; lon < segments; lon++)
		{
			int current = lat * (segments + 1) + lon;
			int next = current + segments + 1;
			indices.push_back(current);
			indices.push_back(next);
			indices.push_back(current + 1);
			indices.push_back(current + 1);
			indices.push_back(next);
			indices.push_back(next + 1);
		}
	}

	mesh.Init(vertices, indices, _driver);
}

void Sphere::Draw(DXCore& _driver) { mesh.Draw(_driver); }

Model::Model(std::string _location, DXCore& _driver)
{
	GEMLoader::GEMModelLoader loader;
	std::vector<GEMLoader::GEMMesh> gemmeshes;
	loader.load(_location, gemmeshes);
	for (int i = 0; i < gemmeshes.size(); i++) {
		Mesh mesh;
		std::vector<STATIC_VERTEX> vertices;
		for (int j = 0; j < gemmeshes[i].verticesStatic.size(); j++) {
			STATIC_VERTEX v;
			memcpy(&v, &gemmeshes[i].verticesStatic[j], sizeof(STATIC_VERTEX));
			vertices.push_back(v);
		}
		mesh.Init(vertices, gemmeshes[i].indices, _driver);
		meshes.push_back(mesh);
	}

}

void Model::Draw(DXCore& _driver)
{
	for (Mesh mesh : meshes)
		mesh.Draw(_driver);
}