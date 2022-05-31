#pragma once
#include "MeshLoader.h"
#include "../Shader/Material.h"
#include "../Transform.h"
namespace Mesh
{
	class Renderer
	{
	protected:
		//Classes
		MeshLoader meshLoader;
		Material mat;
		Shader* shader;
		char* texture_path;


		//Rendering stuff
		GLuint m_program;

		GLuint m_VAO;

		GLuint m_PositionVBO;
		GLuint m_NormalVBO;
		GLuint m_UVVBO;

		GLuint textureID;

		glm::mat4 m_objMat;

	public:
		Transform* transform;

		glm::vec3 lightDir;

	glm::vec4 lightColor;
		glm::vec4 ambientColor;
		glm::vec4 objectColor;


		float ambientStrength;
		float specularStrength;
		int specularIntensity;

		virtual void SetMaterial(Material newMat) = 0;
		virtual void Init() = 0;
		virtual void Render() = 0;
		virtual void Cleanup() = 0;
		virtual bool HasTexture() = 0;
	};

}
