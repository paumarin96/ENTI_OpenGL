#pragma once
#include <glm/glm.hpp>
#include <vector>

namespace Mesh
{
    class MeshLoader
    {
    private:
        const char* path;
        std::vector<glm::vec3> out_vertices;
        std::vector<glm::vec2> out_uvs;
        std::vector<glm::vec3> out_normals;

    public:
        MeshLoader();
        MeshLoader(const char* path);

        std::vector<glm::vec3>* out_vertices1()
        {
            return &out_vertices;
        }

        std::vector<glm::vec2>* out_uvs1()
        {
            return &out_uvs;
        }

        std::vector<glm::vec3>* out_normals1()
        {
            return &out_normals;
        }

        bool loadObj();

        ~MeshLoader();
    };
}

