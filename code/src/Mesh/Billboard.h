#pragma once
#include "MeshLoader.h"
#include "Renderer.h"
#include "../Shader/Material.h"
#include "..\stb_image.h"
#include "../Transform.h"

namespace Mesh {

	class Billboard : public Renderer
	{
	public:



		Billboard(Material mat, const char* path, Transform* _transform);
		Billboard(Material mat, const char* path,  char* texture_Path, Transform* _transform);

		void SetMaterial(Material newMat) override { mat = newMat; }
		void Init() override;
		void Render() override;
		void Cleanup() override;
		bool HasTexture() override {
			return texture_path != nullptr;
		}
	};

}

