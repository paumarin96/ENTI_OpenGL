#pragma once
#include "Object.h"
#include "..\Mesh\Billboard.h"
#include "..\Transform.h"
class BillboardObject : public Object
{
private:
    Mesh::Billboard m_mesh;
public:
    Transform transform;
    glm::vec4 color;

    Mesh::Billboard& Mesh()
    {
	    return m_mesh;
    }

    BillboardObject(Material mat, const char* meshPath, std::string name);
    BillboardObject(Material mat, const char* meshPath, char* texturePath, std::string name);

    void Start() override;
    void Update(float dt) override;
    void Render() override;
    void Cleanup() override;

    ~BillboardObject();

};

