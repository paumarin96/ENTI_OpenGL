#include "BillboardObject.h"

BillboardObject::BillboardObject(Material mat, const char* meshPath, std::string name): transform(Transform{ glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1) }),
							m_mesh(mat,meshPath, & transform), Object(name)
{

}

BillboardObject::BillboardObject(Material mat, const char* meshPath, char* texturePath, std::string name) : transform(Transform{ glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1) }),
								m_mesh(mat, meshPath, texturePath, &transform), Object(name)
{
	
}


void BillboardObject::Start()
{
	m_mesh.Init();
}

void BillboardObject::Update(float dt)
{

}

void BillboardObject::Render()
{
	m_mesh.objectColor = color;
	m_mesh.Render();
}

void BillboardObject::Cleanup()
{
	m_mesh.Cleanup();
}
BillboardObject::~BillboardObject()
{

}
