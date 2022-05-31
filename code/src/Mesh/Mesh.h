#pragma once
#include "MeshLoader.h"
#include "Renderer.h"
#include "../Shader/Material.h"
#include "../Transform.h"

namespace Mesh {

	class Mesh : public Renderer
	{




	public:

		bool outsideTexture;
		GLuint textureId;

		Mesh(Material mat, const char* path, Transform* _transform);
		Mesh(Material mat, const char* path,  char* texture_Path, Transform* _transform);

		void SetMaterial(Material newMat) override { mat = newMat; }
		void Init() override;
		void Render() override;
		void Cleanup() override;
		void SetOutsideTexture(GLuint texture_Id)
		{
			outsideTexture = true;

			textureID = texture_Id;
		}
		bool HasTexture() override
		{
			return texture_path != nullptr;
		}


	};

}

