#include "Material.h"

Material::Material(const char* vertexShaderPath, const char* fragShaderPath) : m_shader(vertexShaderPath, fragShaderPath)
{
	program = m_shader.Program();
}
Material::Material(const char* vertexShaderPath, const char* fragShaderPath, const char* geometryShaderPath) : m_shader(vertexShaderPath, fragShaderPath, geometryShaderPath)
{
	program = m_shader.Program();
}

void Material::UseMaterial()
{
	glUseProgram(program);
}


void Material::Cleanup()
{
	glDeleteProgram(program);
}

Material::~Material()
{

}
