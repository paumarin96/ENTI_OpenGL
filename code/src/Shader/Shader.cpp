#include "Shader.h"
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>

GLuint m_compileShader(const char* shaderStr, GLenum shaderType, const char* name = "") {
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderStr, NULL);
	glCompileShader(shader);
	GLint res;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &res);
	if (res == GL_FALSE) {
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &res);
		char* buff = new char[res];
		glGetShaderInfoLog(shader, res, &res, buff);
		fprintf(stderr, "Error Shader %s: %s", name, buff);
		delete[] buff;
		glDeleteShader(shader);
		return 0;
	}
	return shader;
}
void m_linkProgram(GLuint program) {
	glLinkProgram(program);
	GLint res;
	glGetProgramiv(program, GL_LINK_STATUS, &res);
	if (res == GL_FALSE) {
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &res);
		char* buff = new char[res];
		glGetProgramInfoLog(program, res, &res, buff);
		fprintf(stderr, "Error Link: %s", buff);
		delete[] buff;
	}
}

Shader::Shader(const char* vertPath, const char* fragPath)
{
	vertShader = "";
	fragShader = "";
	std::ifstream vertexFile(vertPath);
	std::ifstream fragmentFile(fragPath);

	if(vertexFile)
	{
		std::ostringstream ssVertex;
		ssVertex << vertexFile.rdbuf();
		vertShader = ssVertex.str();
	}
	if (fragmentFile)
	{
		std::ostringstream ssFragment;
		ssFragment << fragmentFile.rdbuf();
		fragShader = ssFragment.str();

	}


	vertexFile.close();
	fragmentFile.close();

	CompileShader();
	CreateProgram();
	CleanupShaders();
}

Shader::Shader(const char* vertPath, const char* fragPath, const char* geomPath)
{
	vertShader = "";
	fragShader = "";
	geometryShader = "";
	std::ifstream vertexFile(vertPath);
	std::ifstream fragmentFile(fragPath);
	std::ifstream geometryFile(geomPath);

	if (vertexFile)
	{
		std::ostringstream ssVertex;
		ssVertex << vertexFile.rdbuf();
		vertShader = ssVertex.str();
	}
	if (fragmentFile)
	{
		std::ostringstream ssFragment;
		ssFragment << fragmentFile.rdbuf();
		fragShader = ssFragment.str();

	}
	if (geometryFile)
	{
		std::ostringstream ssGeometry;
		ssGeometry << geometryFile.rdbuf();
		geometryShader = ssGeometry.str();

	}


	vertexFile.close();
	fragmentFile.close();
	geometryFile.close();

	CompileShader();
	CreateProgram();
	CleanupShaders();
}

void Shader::CompileShader()
{
	vertexShaderID = m_compileShader(vertShader.data(), GL_VERTEX_SHADER, "vertex shader");
	fragmentShaderID = m_compileShader(fragShader.data(), GL_FRAGMENT_SHADER, "fragment shader");
	if(geometryShader.size() != 0)
	{
		geometryShaderID = m_compileShader(geometryShader.data(), GL_GEOMETRY_SHADER, "geometry shader");
	}

}

void Shader::CreateProgram()
{
	program = glCreateProgram();
	glAttachShader(program, vertexShaderID);
	glAttachShader(program, fragmentShaderID);
	if (geometryShader.size() != 0)
	{
		glAttachShader(program, geometryShaderID);
	}
	glLinkProgram(program);

}

void Shader::CleanupShaders()
{
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	if (geometryShader.size() != 0)
	{
		glDeleteShader(geometryShaderID);
	}
}



Shader::~Shader()
{
}

