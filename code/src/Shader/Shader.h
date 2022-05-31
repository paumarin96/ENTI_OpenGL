#pragma once
#include <string>
#include <GL/glew.h>

class Shader
{
private:

	std::string fragShader;
	std::string vertShader;
	std::string geometryShader;

	GLuint program;
public:
	GLuint Program()
	{
		return program;
	}

private:
	GLuint vertexShaderID;
	GLuint geometryShaderID;
	GLuint fragmentShaderID;

public:
	Shader() = default;
	Shader(const char* vertPath, const char* fragPath);
	Shader(const char* vertPath, const char* fragPath, const char* geomPath);
	const std::string GetFragmentShader()
	{
		return fragShader;
	}

	const std::string GetVertexShader()
	{
		return vertShader;
	}

	const std::string GetGeometryShader()
	{
		return vertShader;
	}
	~Shader();

private:
	void CompileShader();
	void CreateProgram();
	void CleanupShaders();

};
