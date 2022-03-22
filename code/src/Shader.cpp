#include "Shader.h"
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
Shader::Shader(const char* vertPath, const char* fragPath)
{
	std::ifstream vertexFile("vertPath");
	std::ifstream fragmentFile("fragPath");

	if(vertexFile)
	{
		std::ostringstream ssVertex;
		ssVertex << vertexFile.rdbuf();
	}

}
