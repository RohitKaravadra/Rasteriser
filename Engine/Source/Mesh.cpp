#include "Mesh.h"
#include "GEMLoader.h"
#include "Texture.h"

STATIC_VERTEX addVertex(Vec3 p, Vec3 n, float tu, float tv)
{
	STATIC_VERTEX v;
	v.pos = p;
	v.normal = n;

	ShadingFrame frame(v.normal);
	v.tangent = frame.tangent;

	v.tu = tu;
	v.tv = tv;
	return v;
}

void MeshData::Init(void* vertices, int vertexSizeInBytes, int numVertices,
	unsigned int* indices, int numIndices, DXCore* _driver)
{
	D3D11_BUFFER_DESC bd;
	memset(&bd, 0, sizeof(D3D11_BUFFER_DESC));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(unsigned int) * numIndices;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data;
	memset(&data, 0, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = indices;
	_driver->device->CreateBuffer(&bd, &data, &indexBuffer);

	bd.ByteWidth = vertexSizeInBytes * numVertices;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	data.pSysMem = vertices;
	_driver->device->CreateBuffer(&bd, &data, &vertexBuffer);

	indicesSize = numIndices;
	strides = vertexSizeInBytes;
}

MeshData::MeshData()
{
	vertexBuffer = nullptr;
	indexBuffer = nullptr;
}

MeshData::MeshData(const MeshData& other)
{
	indexBuffer = nullptr;
	vertexBuffer = nullptr;
	Copy(other);
}

void MeshData::Init(std::vector<STATIC_VERTEX> vertices, std::vector<unsigned int> indices, DXCore* _driver)
{
	Init(&vertices[0], sizeof(STATIC_VERTEX), vertices.size(), &indices[0], indices.size(), _driver);
}

void MeshData::Init(std::vector<ANIMATED_VERTEX> vertices, std::vector<unsigned int> indices, DXCore* _driver)
{
	Init(&vertices[0], sizeof(ANIMATED_VERTEX), vertices.size(), &indices[0], indices.size(), _driver);
}

void MeshData::Draw(DXCore* _driver) const
{
	UINT offsets = 0;
	_driver->devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_driver->devicecontext->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);
	_driver->devicecontext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	_driver->devicecontext->DrawIndexed(indicesSize, 0, 0);
}

void MeshData::Copy(const MeshData& other)
{
	if (other.indexBuffer) {
		other.indexBuffer->AddRef();
		indexBuffer = other.indexBuffer;
	}

	if (other.vertexBuffer) {
		other.vertexBuffer->AddRef();
		vertexBuffer = other.vertexBuffer;
	}

	indicesSize = other.indicesSize;
	strides = other.strides;
}

void MeshData::Free()
{
	if (vertexBuffer != nullptr)
	{
		vertexBuffer->Release();
		vertexBuffer = nullptr;
	}
	if (indexBuffer != nullptr)
	{
		indexBuffer->Release();
		indexBuffer = nullptr;
	}
}

MeshData& MeshData::operator=(const MeshData& other) {
	if (this != &other)
	{
		Free();
		Copy(other);
	}
	return *this;
}

MeshData::~MeshData()
{
	Free();
}

InstancedMeshData::InstancedMeshData() :MeshData()
{
	instanceBuffer = nullptr;
}
InstancedMeshData::InstancedMeshData(const InstancedMeshData& _other) : MeshData(_other) {
	instanceBuffer = nullptr;
	Copy(_other);
}

void InstancedMeshData::Copy(const InstancedMeshData& _other)
{
	if (_other.instanceBuffer)
	{
		_other.instanceBuffer->AddRef();
		instanceBuffer = _other.instanceBuffer;
	}

	instanceSize = _other.instanceSize;
	instancesSize = _other.instancesSize;
	MeshData::Copy(_other);
}

void InstancedMeshData::Copy(const MeshData& _other)
{
	MeshData::Copy(_other);
}

void InstancedMeshData::SetInstanceData(unsigned int _instanceSize, unsigned int _instancesSize, void* _buffer, DXCore* _driver)
{
	instanceSize = _instanceSize;
	instancesSize = _instancesSize;

	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(D3D11_BUFFER_DESC));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = instanceSize * instancesSize;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data;
	memset(&data, 0, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = _buffer;

	HRESULT hr = _driver->device->CreateBuffer(&bufferDesc, &data, &instanceBuffer);
}

void InstancedMeshData::Draw(DXCore* _driver) const
{
	unsigned int strides[2] = { sizeof(STATIC_VERTEX),instanceSize };
	unsigned int offsets[2] = { 0, 0 };
	ID3D11Buffer* buffers[2] = { vertexBuffer, instanceBuffer };

	_driver->devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_driver->devicecontext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	_driver->devicecontext->IASetVertexBuffers(0, 2, buffers, strides, offsets);
	_driver->devicecontext->DrawIndexedInstanced(indicesSize, instancesSize, 0, 0, 0);
}

void InstancedMeshData::Free()
{
	if (instanceBuffer != nullptr)
	{
		instanceBuffer->Release();
		instanceBuffer = nullptr;
	}
}

InstancedMeshData& InstancedMeshData::operator=(const InstancedMeshData& _other)
{
	if (this != &_other)
	{
		Free();
		Copy(_other);
	}
	return *this;
}

InstancedMeshData::~InstancedMeshData()
{
	Free();
};

void Mesh::AddData(std::string _texture, std::string _normal, MeshData _mesh)
{
	int index = 0;
	for (index; index < textureFiles.size(); index++)
		if (textureFiles[index] == _texture)
			break;

	if (index == textureFiles.size())
	{
		textureFiles.push_back(_texture);
		normalFiles.push_back(_normal);

		std::vector<MeshData> meshList;
		meshList.push_back(_mesh);

		meshes.push_back(meshList);
	}
	else
		meshes[index].push_back(_mesh);
}

void Mesh::Init(std::string _location, DXCore* _driver, std::string _textLocation)
{
	GEMLoader::GEMModelLoader loader;
	std::vector<GEMLoader::GEMMesh> gemmeshes;
	loader.load(_location, gemmeshes);
	bool loadTextures = _textLocation.length() > 2;

	for (int i = 0; i < gemmeshes.size(); i++) {
		std::vector<STATIC_VERTEX> vertices;
		for (int j = 0; j < gemmeshes[i].verticesStatic.size(); j++) {
			STATIC_VERTEX v;
			memcpy(&v, &gemmeshes[i].verticesStatic[j], sizeof(STATIC_VERTEX));
			vertices.push_back(v);
		}

		// load texture from mesh
		std::string _text = ExtractName(gemmeshes[i].material.find("diffuse").getValue());
		std::string _norm = ExtractName(gemmeshes[i].material.find("normals").getValue());

		// load textures if location folder is given
		if (loadTextures)
		{
			TextureManager::load(_textLocation + _text);
			TextureManager::load(_textLocation + _norm);
		}

		// create mesh data from vertices and indices
		MeshData meshData{};
		meshData.Init(vertices, gemmeshes[i].indices, _driver);
		// add data to mesh 
		AddData(_text, _norm, meshData);
	}
}

void Mesh::Draw(DXCore* _driver)
{
	for (auto& _data : meshes)
		for (auto& mesh : _data)
			mesh.Draw(_driver);
}

void Mesh::PrintTextures()
{
	for (auto& obj : textureFiles)
		std::cout << obj << std::endl;
	for (auto& obj : normalFiles)
		std::cout << obj << std::endl;
}

void AnimatedMesh::Init(std::string _location, DXCore* _driver, std::string _textLocation)
{
	GEMLoader::GEMModelLoader loader;
	std::vector<GEMLoader::GEMMesh> gemmeshes;
	GEMLoader::GEMAnimation gemanimation;
	loader.load(_location, gemmeshes, gemanimation);
	bool loadTextures = _textLocation.length() > 2;

	for (int i = 0; i < gemmeshes.size(); i++) {
		MeshData mesh;
		std::vector<ANIMATED_VERTEX> vertices;
		for (int j = 0; j < gemmeshes[i].verticesAnimated.size(); j++) {
			ANIMATED_VERTEX v;
			memcpy(&v, &gemmeshes[i].verticesAnimated[j], sizeof(ANIMATED_VERTEX));
			vertices.push_back(v);
		}

		// load texture from mesh
		std::string _text = ExtractName(gemmeshes[i].material.find("diffuse").getValue());
		std::string _norm = ExtractName(gemmeshes[i].material.find("normals").getValue());

		// load textures if location folder is given
		if (loadTextures)
		{
			TextureManager::load(_textLocation + _text);
			TextureManager::load(_textLocation + _norm);
		}

		// create mesh data from vertices and indices
		MeshData meshData{};
		meshData.Init(vertices, gemmeshes[i].indices, _driver);
		// add data to mesh 
		AddData(_text, _norm, meshData);
	}

	for (int i = 0; i < gemanimation.bones.size(); i++)
	{
		Bone bone;
		bone.name = gemanimation.bones[i].name;
		memcpy(&bone.offset, &gemanimation.bones[i].offset, 16 * sizeof(float));
		bone.parentIndex = gemanimation.bones[i].parentIndex;
		animation.skeleton.bones.push_back(bone);
	}

	for (int i = 0; i < gemanimation.animations.size(); i++)
	{
		std::string name = gemanimation.animations[i].name;
		AnimationSequence aseq;
		aseq.ticksPerSecond = gemanimation.animations[i].ticksPerSecond;
		for (int n = 0; n < gemanimation.animations[i].frames.size(); n++)
		{
			AnimationFrame frame;
			for (int index = 0; index < gemanimation.animations[i].frames[n].positions.size(); index++)
			{
				Vec3 p;
				Quaternion q;
				Vec3 s;
				memcpy(&p, &gemanimation.animations[i].frames[n].positions[index], sizeof(Vec3));
				frame.positions.push_back(p);
				memcpy(&q, &gemanimation.animations[i].frames[n].rotations[index], sizeof(Quaternion));
				frame.rotations.push_back(q);
				memcpy(&s, &gemanimation.animations[i].frames[n].scales[index], sizeof(Vec3));
				frame.scales.push_back(s);
			}
			aseq.frames.push_back(frame);
		}
		animation.animations.insert({ name, aseq });
	}
}


void InstancedMesh::AddData(std::string _texture, std::string _normal, InstancedMeshData _mesh)
{
	int index = 0;
	for (index; index < textureFiles.size(); index++)
		if (textureFiles[index] == _texture)
			break;

	if (index == textureFiles.size())
	{
		textureFiles.push_back(_texture);
		normalFiles.push_back(_normal);

		std::vector<InstancedMeshData> meshList;
		meshList.push_back(_mesh);

		meshes.push_back(meshList);
	}
	else
		meshes[index].push_back(_mesh);
}


void InstancedMesh::Init(std::string _location, DXCore* _driver, std::string _textLocation)
{
	GEMLoader::GEMModelLoader loader;
	std::vector<GEMLoader::GEMMesh> gemmeshes;
	loader.load(_location, gemmeshes);
	bool loadTextures = _textLocation.length() > 2;

	for (int i = 0; i < gemmeshes.size(); i++) {
		std::vector<STATIC_VERTEX> vertices;
		for (int j = 0; j < gemmeshes[i].verticesStatic.size(); j++) {
			STATIC_VERTEX v;
			memcpy(&v, &gemmeshes[i].verticesStatic[j], sizeof(STATIC_VERTEX));
			vertices.push_back(v);
		}

		// load texture from mesh
		std::string _text = ExtractName(gemmeshes[i].material.find("diffuse").getValue());
		std::string _norm = ExtractName(gemmeshes[i].material.find("normals").getValue());

		// load textures if location folder is given
		if (loadTextures)
		{
			TextureManager::load(_textLocation + _text);
			TextureManager::load(_textLocation + _norm);
		}

		// create mesh data from vertices and indices
		InstancedMeshData meshData{};
		meshData.Init(vertices, gemmeshes[i].indices, _driver);
		// add data to mesh 
		AddData(_text, _norm, meshData);
	}
}

void InstancedMesh::SetInstanceData(unsigned int _instanceSize, unsigned int _instancesSize, void* _buffer, DXCore* _driver)
{
	for (auto& _data : meshes)
		for (auto& mesh : _data)
			mesh.SetInstanceData(_instanceSize, _instancesSize, _buffer, _driver);
}

void InstancedMesh::Draw(DXCore* _driver)
{
	for (auto& _data : meshes)
		for (auto& mesh : _data)
			mesh.Draw(_driver);
}

void InstancedMesh::PrintTextures()
{
	for (auto& obj : textureFiles)
		std::cout << obj << std::endl;
	for (auto& obj : normalFiles)
		std::cout << obj << std::endl;
}