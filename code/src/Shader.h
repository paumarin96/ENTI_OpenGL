#pragma once
#include <GL/glew.h>

class Shader
{

public:
	Shader(const char* vertPath, const char* fragPath);
	GLchar* GetFragmentShader();
	GLchar* GetVertexShader();
	~Shader();
private:
	char* vertPath; 
	char* fragPath; 
	GLchar* fragShader;
	GLchar* vertShader;
};
