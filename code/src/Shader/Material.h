#pragma once
#include "Shader.h"
#include "glm/glm.hpp"
class Material
{
private:
	Shader m_shader;
	GLuint program;

public:

	Material() = default;
	Material(const char* vertexShaderPath, const char* fragShaderPath);
	Material(const char* vertexShaderPath, const char* fragShaderPath, const char* geometryShaderPath);

	void UseMaterial();
	void Draw();
	void Cleanup();

	Shader* GetShader()
	{
		return &m_shader;
	}

	~Material();
};

