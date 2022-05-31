#pragma once
#include "MeshLoader.h"
#include "Renderer.h"
#include "../Shader/Material.h"
#include "../Transform.h"


namespace Mesh {

	class CastleMap : public Renderer
	{
	private:
		const char* map_texture_path;

		MeshLoader map_uvs;
		GLuint vbo_map_uvs;

		GLuint map_textureID;
		GLuint water_textureID;
		GLuint grass_textureID;
		GLuint sand_textureID;
		GLuint rock_textureID;


	public:


		CastleMap(Material mat, const char* path, Transform* _transform);
		CastleMap(Material mat, const char* path,  const char* path_to_map_uvs, char* texture_Path, char* texture_map_path, Transform* _transform);

		void SetMaterial(Material newMat) override { mat = newMat; }
		void Init() override;
		void Render() override;
		void Cleanup() override;
		bool HasTexture() override
		{
			return texture_path != nullptr;
		}


	};

}

