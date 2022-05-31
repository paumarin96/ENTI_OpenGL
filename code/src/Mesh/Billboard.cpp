#include "Billboard.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "glm/glm.hpp"
#include "../Lighting/Lighting.h"

#include "..\stb_image.h"

#include <SDL_timer.h>
#include <imgui/imgui.h>




#define lighting Lighting::Instance()

namespace RenderVars
{
	extern glm::mat4 _modelView;
	extern glm::mat4 _MVP;
}

Mesh::Billboard::Billboard(Material mat, const char* path, Transform* _transform) : Renderer()
{
	Renderer::mat = mat;
	Renderer::transform = _transform;
	meshLoader = MeshLoader(path);
	meshLoader.loadObj();
	
	
}

Mesh::Billboard::Billboard(Material mat, const char* path,  char* texture_Path, Transform* _transform) : Renderer()
{
	Renderer::mat = mat;
	Renderer::transform = _transform;
	meshLoader = MeshLoader(path);
	meshLoader.loadObj();
	texture_path = texture_Path;

}


void Mesh::Billboard::Init()
{
	int x, y, n;
	unsigned char* data = nullptr;
	if(HasTexture())
	{
		stbi_set_flip_vertically_on_load(true);

		data = stbi_load(texture_path, &x, &y, &n, 4);
	}


	shader = mat.GetShader();
	m_program = shader->Program();

	lightDir = glm::vec3(-1, -1, 0);
	lightColor = glm::vec4(1, 0, 0, 1);
	ambientColor = glm::vec4(0.25, 0.25, 0.25, 1);

	specularIntensity = 32;
	specularStrength = 0.5f;
	//Create the vertex array object
//This object maintains the state related to the input of the OpenGL
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_PositionVBO); //Create the vertex buffer object.
	glBindBuffer(GL_ARRAY_BUFFER, m_PositionVBO);

	glBufferData(GL_ARRAY_BUFFER, meshLoader.out_vertices1()->size() * sizeof(glm::vec3), &(*meshLoader.out_vertices1())[0], GL_STATIC_DRAW);


	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(float),
		(void*)0
	);



	glGenBuffers(1, &m_NormalVBO); //Create the vertex buffer object.
	glBindBuffer(GL_ARRAY_BUFFER, m_NormalVBO);

	glBufferData(GL_ARRAY_BUFFER, meshLoader.out_normals1()->size() * sizeof(glm::vec3), &(*meshLoader.out_normals1())[0], GL_STATIC_DRAW);


	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(float),
		(void*)0
	);


	glGenBuffers(1, &m_UVVBO); //Create the vertex buffer object.
	glBindBuffer(GL_ARRAY_BUFFER, m_UVVBO);

	glBufferData(GL_ARRAY_BUFFER, meshLoader.out_uvs1()->size() * sizeof(glm::vec2), &(*meshLoader.out_uvs1())[0], GL_STATIC_DRAW);


	glVertexAttribPointer(
		2,
		2,
		GL_FLOAT,
		GL_FALSE,
		2 * sizeof(float),
		(void*)0
	);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	if(HasTexture())
	{
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		stbi_image_free(data);

	}


	
}


 
void Mesh::Billboard::Render()
{
	glUseProgram(m_program);

	glm::mat4 translation = glm::translate(glm::mat4(), transform->position);
	glm::mat4 scale = glm::scale(glm::mat4(), transform->scale);

	m_objMat = translation * scale;

	//Transform matrices
	glUniformMatrix4fv(glGetUniformLocation(m_program, "objMat"), 1, GL_FALSE, glm::value_ptr(m_objMat));
	glUniformMatrix4fv(glGetUniformLocation(m_program, "mv_Mat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_modelView));
	glUniformMatrix4fv(glGetUniformLocation(m_program, "mvpMat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_MVP));
	glUniform1f(glGetUniformLocation(m_program, "time"), ImGui::GetTime());
	
	//Directional light;
	glUniform4f(glGetUniformLocation(m_program, "directionalLight.color"), lighting->directionalLight.color[0], lighting->directionalLight.color[1], lighting->directionalLight.color[2], lighting->directionalLight.color[3]);
	glUniform4f(glGetUniformLocation(m_program, "directionalLight.ambientColor"), lighting->directionalLight.ambientColor[0], lighting->directionalLight.ambientColor[1], lighting->directionalLight.ambientColor[2], lighting->directionalLight.ambientColor[3]);
	glUniform1f(glGetUniformLocation(m_program, "directionalLight.ambientStrength"), lighting->directionalLight.ambientStrength);
	glUniform3f(glGetUniformLocation(m_program, "directionalLight.direction"), lighting->directionalLight.direction[0], lighting->directionalLight.direction[1], lighting->directionalLight.direction[2]);


	glUniform1i(glGetUniformLocation(m_program, "pointLightCount"), lighting->pointLights.size());
	//Point light;
	for (int i = 0; i < lighting->pointLights.size(); ++i)
	{
		std::string base("pointLight[" + std::to_string(i) + "]");
		glUniform4f(glGetUniformLocation(m_program, std::string(base + ".color").c_str()), lighting->pointLights[i].color[0], lighting->pointLights[i].color[1], lighting->pointLights[i].color[2], lighting->pointLights[i].color[3]);
		glUniform4f(glGetUniformLocation(m_program, std::string(base + ".ambientColor").c_str()), lighting->pointLights[i].ambientColor[0], lighting->pointLights[i].ambientColor[1], lighting->pointLights[i].ambientColor[2], lighting->pointLights[i].ambientColor[3]);
		glUniform1f(glGetUniformLocation(m_program, std::string(base + ".ambientStrength").c_str()), lighting->pointLights[i].ambientStrength);
		glUniform3f(glGetUniformLocation(m_program, std::string(base + ".position").c_str()), lighting->pointLights[i].position[0], lighting->pointLights[i].position[1], lighting->pointLights[i].position[2]);
		glUniform1f(glGetUniformLocation(m_program, std::string(base + ".strength").c_str()), lighting->pointLights[i].strength);
		glUniform1f(glGetUniformLocation(m_program, std::string(base + ".constant").c_str()), lighting->pointLights[i].constant);
		glUniform1f(glGetUniformLocation(m_program, std::string(base + ".linear").c_str()), lighting->pointLights[i].linear);
		glUniform1f(glGetUniformLocation(m_program, std::string(base + ".quadratic").c_str()), lighting->pointLights[i].quadratic);
		glUniform1i(glGetUniformLocation(m_program, std::string(base + ".isActive").c_str()), lighting->pointLights[i].isActive);
	}

	glUniform1i(glGetUniformLocation(m_program, "spotLightCount"), lighting->spotLights.size());
	//Spotlight;
	for (int i = 0; i < lighting->spotLights.size(); ++i)
	{
		std::string base("spotLight[" + std::to_string(i) + "]");
		glUniform4f(glGetUniformLocation(m_program, std::string(base + ".color").c_str()), lighting->spotLights[i].color[0], lighting->spotLights[i].color[1], lighting->spotLights[i].color[2], lighting->spotLights[i].color[3]);
		glUniform4f(glGetUniformLocation(m_program, std::string(base + ".ambientColor").c_str()), lighting->spotLights[i].ambientColor[0], lighting->spotLights[i].ambientColor[1], lighting->spotLights[i].ambientColor[2], lighting->spotLights[i].ambientColor[3]);
		glUniform1f(glGetUniformLocation(m_program, std::string(base + ".ambientStrength").c_str()), lighting->spotLights[i].ambientStrength);
		glUniform3f(glGetUniformLocation(m_program, std::string(base + ".position").c_str()), lighting->spotLights[i].position[0], lighting->spotLights[i].position[1], lighting->spotLights[i].position[2]);
		glUniform3f(glGetUniformLocation(m_program, std::string(base + ".direction").c_str()), lighting->spotLights[i].direction[0], lighting->spotLights[i].direction[1], lighting->spotLights[i].direction[2]);
		glUniform1f(glGetUniformLocation(m_program, std::string(base + ".strength").c_str()), lighting->spotLights[i].strength);
		glUniform1f(glGetUniformLocation(m_program, std::string(base + ".cutoff").c_str()), glm::cos(lighting->spotLights[i].cutOff));
		glUniform1f(glGetUniformLocation(m_program, std::string(base + ".outerCutoff").c_str()), glm::cos(lighting->spotLights[i].outerCutoff));
		glUniform1i(glGetUniformLocation(m_program, std::string(base + ".isActive").c_str()), lighting->spotLights[i].isActive);
	}

	//Material properties
	glm::vec3 cameraPosition = glm::inverse(RenderVars::_modelView) * glm::vec4(0, 0, 0, 1);
	glUniform3f(glGetUniformLocation(m_program, "viewPos"), cameraPosition[0], cameraPosition[1], cameraPosition[2]);
	glUniform4f(glGetUniformLocation(m_program, "objectColor"), objectColor[0], objectColor[1], objectColor[2], objectColor[3]);
	glUniform1f(glGetUniformLocation(m_program, "specularStrength"), specularStrength);
	glUniform1i(glGetUniformLocation(m_program, "specularIntensity"), specularIntensity);



	glBindVertexArray(m_VAO);

	if(HasTexture())
	{
		glUniform1i(glGetUniformLocation(m_program, "hasTexture"), 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glUniform1i(glGetUniformLocation(m_program, "diffuseTexture"), 1);
	}


	auto vertNumber = meshLoader.out_vertices1()->size();
	glDrawArrays(GL_POINTS, 0, vertNumber);
}

void Mesh::Billboard::Cleanup()
{

	mat.Cleanup();
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteTextures(1, &textureID);

}

