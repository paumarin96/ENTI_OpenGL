#pragma once
#include "Object.h"
#include "..\Mesh\Mesh.h"
#include "..\Transform.h"
class MeshObject : public Object
{
private:
    Mesh::Renderer* m_mesh;
public:
    Transform transform;
    glm::vec4 color;

    Mesh::Renderer& Mesh()
    {
	    return *m_mesh;
    }

    MeshObject(Material mat, const char* meshPath, std::string name);
    MeshObject(Material mat, const char* meshPath, char* texturePath, std::string name);
    MeshObject(Mesh::Renderer* mesh, Material mat, std::string name);

    void Start() override;
    void Update(float dt) override;
    void Render() override;
    void Cleanup() override;

    ~MeshObject();

};

