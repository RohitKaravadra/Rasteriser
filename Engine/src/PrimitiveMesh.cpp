#include "../hdr/Mesh.h"

MeshData* Primitives::Cube(DXCore* _driver)
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

	MeshData* mesh = new MeshData();
	mesh->Init(vertices, indices, _driver);
	return mesh;
}

MeshData* Primitives::Plane(DXCore* _driver)
{
	std::vector<STATIC_VERTEX> vertices;
	vertices.push_back(addVertex(Vec3(-5, 0, -5), Vec3(0, 1, 0), 0, 0));
	vertices.push_back(addVertex(Vec3(5, 0, -5), Vec3(0, 1, 0), 1, 0));
	vertices.push_back(addVertex(Vec3(5, 0, 5), Vec3(0, 1, 0), 1, 1));
	vertices.push_back(addVertex(Vec3(-5, 0, 5), Vec3(0, 1, 0), 0, 1));

	std::vector<unsigned int> indices;
	indices.push_back(0); indices.push_back(1); indices.push_back(2);
	indices.push_back(0); indices.push_back(2); indices.push_back(3);

	MeshData* mesh = new MeshData();
	mesh->Init(vertices, indices, _driver);
	return mesh;
}

MeshData* Primitives::Sphere(unsigned int _rings, unsigned int _segments, 
	unsigned int _radius, DXCore* _driver)
{
	std::vector<STATIC_VERTEX> vertices;
	for (int lat = 0; lat <= _rings; lat++) 
	{
		float theta    = lat * PI / _rings;
		float sinTheta = sinf(theta);
		float cosTheta = cosf(theta);

		for (int lon = 0; lon <= _segments; lon++) 
		{
			float phi    = lon * PI2 / _segments;
			float sinPhi = sinf(phi);
			float cosPhi = cosf(phi);

			Vec3 position(_radius * sinTheta * cosPhi, _radius * cosTheta, _radius * sinTheta * sinPhi);
			Vec3 normal = position.Normalize();

			float tu = 1.0f - (float)lon / _segments;
			float tv = 1.0f - (float)lat / _rings;

			vertices.push_back(addVertex(position, normal, tu, tv));
		}
	}

	std::vector<unsigned int> indices;

	for (int lat = 0; lat < _rings; lat++)
	{
		for (int lon = 0; lon < _segments; lon++)
		{
			int current = lat * (_segments + 1) + lon;
			int next    = current + _segments + 1;
	
			indices.push_back(current);
			indices.push_back(next);
			indices.push_back(current + 1);
			indices.push_back(current + 1);
			indices.push_back(next);
			indices.push_back(next + 1);
		}
	}

	MeshData* mesh = new MeshData();
	mesh->Init(vertices, indices, _driver);
	return mesh;
}

MeshData* Primitives::BillBoard(DXCore* _driver, float _scale)
{
	std::vector<STATIC_VERTEX> vertices;
	Vec3 p0 = Vec3(-.5f, 1.0f, 0.0f) * _scale;
	Vec3 p1 = Vec3(.5f, 1.0f, 0.0f) * _scale;
	Vec3 p2 = Vec3(.5f, 0.0f, 0.0f) * _scale;
	Vec3 p3 = Vec3(-.5f, 0.0f, 0.0f) * _scale;

	vertices.push_back(addVertex(p0, Vec3(0.0f, 0.0f, -1.0f), 0.0f, 0.0f));
	vertices.push_back(addVertex(p1, Vec3(0.0f, 0.0f, -1.0f), 1.0f, 0.0f));
	vertices.push_back(addVertex(p2, Vec3(0.0f, 0.0f, -1.0f), 1.0f, 1.0f));
	vertices.push_back(addVertex(p3, Vec3(0.0f, 0.0f, -1.0f), 0.0f, 1.0f));

	std::vector<unsigned int> indices;
	indices.push_back(0); indices.push_back(1); indices.push_back(2);
	indices.push_back(0); indices.push_back(2); indices.push_back(3);

	MeshData* mesh = new MeshData();
	mesh->Init(vertices, indices, _driver);
	return mesh;
}