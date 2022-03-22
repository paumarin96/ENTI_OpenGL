#pragma once
#include "Mesh.h"
#include "RenderVars.h"
namespace Utils
{
	GLuint compileShader(const char* shaderStr, GLenum shaderType, const char* name = "") {
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
	void linkProgram(GLuint program) {
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
}
namespace Exercise
{
	GLuint program;
	GLuint VAO;
	GLuint VBO;


	//Vertex shader
	static const GLchar* vertex_shader_source[] = {
		"#version 330\n"
		"layout (location = 0) in vec3 aPos;"
		"void main(){\n"

			"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}"

	};
	//Fragment shader
	static const GLchar* fragment_shader_source[] = {
		"#version 330\n"
		"\n"
		"uniform vec4 cpuColor;\n"
		"out vec4 color;\n"
		"void main(){\n"
			"color = cpuColor;\n"
		"}"
	};

	void init()
	{
		GLuint vertex_shader;
		GLuint fragment_shader;

		constexpr float vertices[] = {
		0.5f, -0.5f, 0.0f,
		 0.5f, 0.5f, 0.0f,
			-0.5f,  -0.5f, 0.0f,
		 -0.5f,  0.5f, 0.0f
		};

		vertex_shader = glCreateShader(GL_VERTEX_SHADER); //dona'm un id per un shader. La GPU retorna un ID no utilitzat per aquest shader
		glShaderSource(vertex_shader, 1, vertex_shader_source, NULL); //pugem el codi a la GPU
		glCompileShader(vertex_shader); //Compilem el shader (o operem dades)
		//Bind
		//Destroy


		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, fragment_shader_source, NULL); //pugem el codi a la GPU
		// Operem el shader
		glCompileShader(fragment_shader);

		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);


		//Create the vertex array object
		//This object maintains the state related to the input of the OpenGL
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		
		glGenBuffers(1, &VBO); //Create the vertex buffer object.
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			3 * sizeof(float),
			(void*)0
		);

		glEnableVertexAttribArray(0);
		glBindVertexArray(0);
	}
	void cleanup()
	{
		glDeleteProgram(program);
		glDeleteVertexArrays(1, &VAO);
	}
	void render()

	{

		time_t currentTime = SDL_GetTicks() / 1000;
		glPointSize(40.0f);
		glBindVertexArray(VAO);

		GLuint uniformId = glGetUniformLocation(program,"cpuColor");
		glUniform4f(uniformId, 0.4f + (float)sin(currentTime)*0.5f, 0.7f + (float)cos(currentTime) * 0.5f, 0.3f, 1);

		glUseProgram(program);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
}

namespace OBJLoading
{
	GLuint program;
	GLuint VAO;
	GLuint VBO;

	GLuint NormalVAO;
	GLuint NormalVBO;
	glm::mat4 objMat;
	Mesh mesh;

	glm::vec4 lightColor;
	glm::vec4 objectColor;

	//Vertex shader
	static const char* vertex_shader_source ="#version 330\n\
		layout (location = 0) in vec3 in_Position;\n\
		layout (location = 1) in vec3 in_Normal;\n\
		out vec4 vert_Normal;\n\
		uniform mat4 objMat;\n\
		uniform mat4 mv_Mat;\n\
		uniform mat4 mvpMat;\n\
		void main() {\n\
			gl_Position = mvpMat * objMat * vec4(in_Position, 1.0);\n\
			vert_Normal = mv_Mat * objMat * vec4(in_Normal, 0.0);\n\
		}";
	//Fragment shader
	static const char* fragment_shader_source[] = {
		"#version 330\n"
		"\n"
		"in vec4 vert_Normal;\n"
		"out vec4 color;\n"
		"void main(){\n"
			"color = vert_Normal;\n"
		"}"
	};


	void init()
	{
		mesh = Mesh("basura.obj");
		mesh.loadObj();
		GLuint vertex_shader;
		GLuint fragment_shader;
		objMat = glm::mat4(1.f);
		vertex_shader = glCreateShader(GL_VERTEX_SHADER); //dona'm un id per un shader. La GPU retorna un ID no utilitzat per aquest shader
		glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL); //pugem el codi a la GPU
		glCompileShader(vertex_shader); //Compilem el shader (o operem dades)
		//Bind
		//Destroy


		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, fragment_shader_source, NULL); //pugem el codi a la GPU
		// Operem el shader
		glCompileShader(fragment_shader);

		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);


		//Create the vertex array object
		//This object maintains the state related to the input of the OpenGL
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO); //Create the vertex buffer object.
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, mesh.out_vertices1()->size()  * sizeof(glm::vec3), &(*mesh.out_vertices1())[0], GL_STATIC_DRAW);


		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			3 * sizeof(float),
			(void*)0
		);

	

		glGenBuffers(1, &NormalVBO); //Create the vertex buffer object.
		glBindBuffer(GL_ARRAY_BUFFER, NormalVBO);

		glBufferData(GL_ARRAY_BUFFER, mesh.out_normals1()->size() * sizeof(glm::vec3), &(*mesh.out_normals1())[0], GL_STATIC_DRAW);


		glVertexAttribPointer(
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			3 * sizeof(float),
			(void*)0
		);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindVertexArray(0);
	}
	void cleanup()
	{
		glDeleteProgram(program);
		glDeleteVertexArrays(1, &VAO);
	}
	void render()

	{
		glUseProgram(program);

		glUniformMatrix4fv(glGetUniformLocation(program, "objMat"), 1, GL_FALSE, glm::value_ptr(objMat));
		glUniformMatrix4fv(glGetUniformLocation(program, "mv_Mat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_modelView));
		glUniformMatrix4fv(glGetUniformLocation(program, "mvpMat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_MVP));
		time_t currentTime = SDL_GetTicks() / 1000;
		glPointSize(40.0f);
		glBindVertexArray(VAO);
		//glBindVertexArray(NormalVAO);

		GLuint uniformId = glGetUniformLocation(program, "cpuColor");
		glUniform4f(uniformId, 0.4f + (float)sin(currentTime) * 0.5f, 0.7f + (float)cos(currentTime) * 0.5f, 0.3f, 1);
		auto vertNumber = mesh.out_vertices1()->size();
		glDrawArrays(GL_TRIANGLES, 0, vertNumber);
	}
}
