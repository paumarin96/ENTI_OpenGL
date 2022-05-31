#include "MeshObject.h"

MeshObject::MeshObject(Material mat, const char* meshPath, std::string name): transform(Transform{ glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1) }),
						Object(name)
{
	m_mesh = new Mesh::Mesh(mat, meshPath, &transform);
}

MeshObject::MeshObject(Material mat, const char* meshPath, char* texturePath, std::string name) : transform(Transform{ glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1) }),
								Object(name)
{
	m_mesh = new Mesh::Mesh(mat, meshPath, texturePath, &transform);
}

MeshObject::MeshObject(Mesh::Renderer* mesh, Material mat, std::string name) : transform(Transform{ glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1) }),
Object(name)
{
	m_mesh = mesh;
	m_mesh->transform = &transform;

}

void MeshObject::Start()
{
	m_mesh->Init();
}

void MeshObject::Update(float dt)
{

}

void MeshObject::Render()
{
	m_mesh->objectColor = color;
	m_mesh->Render();
}

void MeshObject::Cleanup()
{
	m_mesh->Cleanup();
}
MeshObject::~MeshObject()
{

}
