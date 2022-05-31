#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <SDL_timer.h>

#include <imgui\imgui.h>
#include <imgui\imgui_impl_sdl_gl3.h>

#include "GL_framework.h"
#include "Exercise.h"
#include "RenderVars.h"

#include "Object/MeshObject.h"
#include "Lighting/Lighting.h"
#include "Object/BillboardObject.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Mesh/CastleMap.h"
#include "Mesh/Mesh.h"
#include "Camera/Camera.h"
#include "Spline/Spline.h"

///////// fw decl
namespace ImGui {
	void Render();
}
namespace Axis {
void setupAxis();
void cleanupAxis();
void drawAxis();
}
////////////////



namespace RV = RenderVars;
struct DollyEffect
{

	float screenWidth=800;
	float screenHeight=800;

	float height;
	float length;


	float startingFOV = 19.f;
	float endingFOV = 30.f;
	float currentFov;

	void CalculateStartingHeight(MeshObject& object, Transform& camera)
	{
		length = (camera.position.z - object.transform.position.z);
		height = glm::tan(startingFOV) * length;
		currentFov = startingFOV;
		
	}
};

DollyEffect dolly;

void GLResize(int width, int height) {
	dolly.screenHeight = height;
	dolly.screenWidth = width;
	glViewport(0, 0, width, height);
	if(height != 0) RV::_projection = glm::perspective(RV::FOV, (float)width / (float)height, RV::zNear, RV::zFar);
	else RV::_projection = glm::perspective(RV::FOV, 0.f, RV::zNear, RV::zFar);
}

void GLmousecb(MouseEvent ev) {
	if(RV::prevMouse.waspressed && RV::prevMouse.button == ev.button) {
		float diffx = ev.posx - RV::prevMouse.lastx;
		float diffy = ev.posy - RV::prevMouse.lasty;
		switch(ev.button) {
		case MouseEvent::Button::Left: // ROTATE
			RV::rota[0] += diffx * 0.005f;
			RV::rota[1] += diffy * 0.005f;
			break;
		case MouseEvent::Button::Right: // MOVE XY
			RV::panv[0] += diffx * 0.03f;
			RV::panv[1] -= diffy * 0.03f;
			break;
		case MouseEvent::Button::Middle: // MOVE Z
			RV::panv[2] += diffy * 0.05f;
			break;
		default: break;
		}
	} else {
		RV::prevMouse.button = ev.button;
		RV::prevMouse.waspressed = true;
	}
	RV::prevMouse.lastx = ev.posx;
	RV::prevMouse.lasty = ev.posy;
}

//////////////////////////////////////////////////
GLuint compileShader(const char* shaderStr, GLenum shaderType, const char* name="") {
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderStr, NULL);
	glCompileShader(shader);
	GLint res;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &res);
	if (res == GL_FALSE) {
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &res);
		char *buff = new char[res];
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
		char *buff = new char[res];
		glGetProgramInfoLog(program, res, &res, buff);
		fprintf(stderr, "Error Link: %s", buff);
		delete[] buff;
	}
}

////////////////////////////////////////////////// AXIS
namespace Axis {
GLuint AxisVao;
GLuint AxisVbo[3];
GLuint AxisShader[2];
GLuint AxisProgram;

float AxisVerts[] = {
	0.0, 0.0, 0.0,
	1.0, 0.0, 0.0,
	0.0, 0.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, 0.0,
	0.0, 0.0, 1.0
};
float AxisColors[] = {
	1.0, 0.0, 0.0, 1.0,
	1.0, 0.0, 0.0, 1.0,
	0.0, 1.0, 0.0, 1.0,
	0.0, 1.0, 0.0, 1.0,
	0.0, 0.0, 1.0, 1.0,
	0.0, 0.0, 1.0, 1.0
};
GLubyte AxisIdx[] = {
	0, 1,
	2, 3,
	4, 5
};
const char* Axis_vertShader =
"#version 330\n\
in vec3 in_Position;\n\
in vec4 in_Color;\n\
out vec4 vert_color;\n\
uniform mat4 mvpMat;\n\
void main() {\n\
	vert_color = in_Color;\n\
	gl_Position = mvpMat * vec4(in_Position, 1.0);\n\
}";
const char* Axis_fragShader =
"#version 330\n\
in vec4 vert_color;\n\
out vec4 out_Color;\n\
void main() {\n\
	out_Color = vert_color;\n\
}";

void setupAxis() {
	glGenVertexArrays(1, &AxisVao);
	glBindVertexArray(AxisVao);
	glGenBuffers(3, AxisVbo);

	glBindBuffer(GL_ARRAY_BUFFER, AxisVbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, AxisVerts, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, AxisVbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, AxisColors, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 4, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, AxisVbo[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * 6, AxisIdx, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	AxisShader[0] = compileShader(Axis_vertShader, GL_VERTEX_SHADER, "AxisVert");
	AxisShader[1] = compileShader(Axis_fragShader, GL_FRAGMENT_SHADER, "AxisFrag");

	AxisProgram = glCreateProgram();
	glAttachShader(AxisProgram, AxisShader[0]);
	glAttachShader(AxisProgram, AxisShader[1]);
	glBindAttribLocation(AxisProgram, 0, "in_Position");
	glBindAttribLocation(AxisProgram, 1, "in_Color");
	linkProgram(AxisProgram);
}
void cleanupAxis() {
	glDeleteBuffers(3, AxisVbo);
	glDeleteVertexArrays(1, &AxisVao);

	glDeleteProgram(AxisProgram);
	glDeleteShader(AxisShader[0]);
	glDeleteShader(AxisShader[1]);
}
void drawAxis() {
	glBindVertexArray(AxisVao);
	glUseProgram(AxisProgram);
	glUniformMatrix4fv(glGetUniformLocation(AxisProgram, "mvpMat"), 1, GL_FALSE, glm::value_ptr(RV::_MVP));
	glDrawElements(GL_LINES, 6, GL_UNSIGNED_BYTE, 0);

	glUseProgram(0);
	glBindVertexArray(0);
}
}

////////////////////////////////////////////////// CUBE
namespace Cube {
GLuint cubeVao;
GLuint cubeVbo[3];
GLuint cubeShaders[2];
GLuint cubeProgram;
glm::mat4 objMat = glm::mat4(1.f);

extern const float halfW = 0.5f;
int numVerts = 24 + 6; // 4 vertex/face * 6 faces + 6 PRIMITIVE RESTART

					   //   4---------7
					   //  /|        /|
					   // / |       / |
					   //5---------6  |
					   //|  0------|--3
					   //| /       | /
					   //|/        |/
					   //1---------2
glm::vec3 verts[] = {
	glm::vec3(-halfW, -halfW, -halfW),
	glm::vec3(-halfW, -halfW,  halfW),
	glm::vec3(halfW, -halfW,  halfW),
	glm::vec3(halfW, -halfW, -halfW),
	glm::vec3(-halfW,  halfW, -halfW),
	glm::vec3(-halfW,  halfW,  halfW),
	glm::vec3(halfW,  halfW,  halfW),
	glm::vec3(halfW,  halfW, -halfW)
};
glm::vec3 norms[] = {
	glm::vec3(0.f, -1.f,  0.f),
	glm::vec3(0.f,  1.f,  0.f),
	glm::vec3(-1.f,  0.f,  0.f),
	glm::vec3(1.f,  0.f,  0.f),
	glm::vec3(0.f,  0.f, -1.f),
	glm::vec3(0.f,  0.f,  1.f)
};

glm::vec3 cubeVerts[] = {
	verts[1], verts[0], verts[2], verts[3],
	verts[5], verts[6], verts[4], verts[7],
	verts[1], verts[5], verts[0], verts[4],
	verts[2], verts[3], verts[6], verts[7],
	verts[0], verts[4], verts[3], verts[7],
	verts[1], verts[2], verts[5], verts[6]
};
glm::vec3 cubeNorms[] = {
	norms[0], norms[0], norms[0], norms[0],
	norms[1], norms[1], norms[1], norms[1],
	norms[2], norms[2], norms[2], norms[2],
	norms[3], norms[3], norms[3], norms[3],
	norms[4], norms[4], norms[4], norms[4],
	norms[5], norms[5], norms[5], norms[5]
};
GLubyte cubeIdx[] = {
	0, 1, 2, 3, UCHAR_MAX,
	4, 5, 6, 7, UCHAR_MAX,
	8, 9, 10, 11, UCHAR_MAX,
	12, 13, 14, 15, UCHAR_MAX,
	16, 17, 18, 19, UCHAR_MAX,
	20, 21, 22, 23, UCHAR_MAX
};

const char* cube_vertShader =
"#version 330\n\
in vec3 in_Position;\n\
in vec3 in_Normal;\n\
out vec4 vert_Normal;\n\
uniform mat4 objMat;\n\
uniform mat4 mv_Mat;\n\
uniform mat4 mvpMat;\n\
void main() {\n\
	gl_Position = mvpMat * objMat * vec4(in_Position, 1.0);\n\
	vert_Normal = mv_Mat * objMat * vec4(in_Normal, 0.0);\n\
}";
const char* cube_fragShader =
"#version 330\n\
in vec4 vert_Normal;\n\
out vec4 out_Color;\n\
uniform mat4 mv_Mat;\n\
uniform vec4 color;\n\
void main() {\n\
	out_Color = vec4(color.xyz * dot(vert_Normal, mv_Mat*vec4(0.0, 1.0, 0.0, 0.0)) + color.xyz * 0.3, 1.0 );\n\
}";
void setupCube() {
	glGenVertexArrays(1, &cubeVao);
	glBindVertexArray(cubeVao);
	glGenBuffers(3, cubeVbo);

	glBindBuffer(GL_ARRAY_BUFFER, cubeVbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), cubeVerts, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, cubeVbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeNorms), cubeNorms, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glPrimitiveRestartIndex(UCHAR_MAX);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeVbo[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIdx), cubeIdx, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	cubeShaders[0] = compileShader(cube_vertShader, GL_VERTEX_SHADER, "cubeVert");
	cubeShaders[1] = compileShader(cube_fragShader, GL_FRAGMENT_SHADER, "cubeFrag");

	cubeProgram = glCreateProgram();
	glAttachShader(cubeProgram, cubeShaders[0]);
	glAttachShader(cubeProgram, cubeShaders[1]);
	glBindAttribLocation(cubeProgram, 0, "in_Position");
	glBindAttribLocation(cubeProgram, 1, "in_Normal");
	linkProgram(cubeProgram);
}
void cleanupCube() {
	glDeleteBuffers(3, cubeVbo);
	glDeleteVertexArrays(1, &cubeVao);

	glDeleteProgram(cubeProgram);
	glDeleteShader(cubeShaders[0]);
	glDeleteShader(cubeShaders[1]);
}
void updateCube(const glm::mat4& transform) {
	objMat = transform;
}
void drawCube() {


	time_t currentTime = SDL_GetTicks() / 1000;

	glEnable(GL_PRIMITIVE_RESTART);
	glBindVertexArray(cubeVao);
	glUseProgram(cubeProgram);
	glm::mat4 t = glm::translate(glm::mat4(), glm::vec3(10,10,10));
	objMat = t;
	glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "objMat"), 1, GL_FALSE, glm::value_ptr(objMat));
	glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "mv_Mat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_modelView));
	glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "mvpMat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_MVP));
	glUniform4f(glGetUniformLocation(cubeProgram, "color"), 0.1f, 1.f, 1.f, 0.f);
	glDrawElements(GL_TRIANGLE_STRIP, numVerts, GL_UNSIGNED_BYTE, 0);

	////scaling and translating
	//t = glm::translate(glm::mat4(), glm::vec3(-1.0f, 1.0f * sin(ImGui::GetTime()) * 3.0f, 5.0f));
	//	
	//auto s = glm::scale(glm::mat4(), glm::vec3((1.0f + sin(ImGui::GetTime())/2) * 2.0f, (1.0f + sin(ImGui::GetTime()) / 2) * 2.0f, (1.0f + sin(ImGui::GetTime()) / 2) * 2.0f));
	//objMat =  t * s;


	glm::vec3 myRotationAxis(0, 1, 0); 
	float rotationSpeed{ 1 };
	float angle = ImGui::GetTime() / 2 * rotationSpeed;

	auto r = glm::rotate(glm::mat4(), angle, myRotationAxis) * glm::translate(glm::mat4(), glm::vec3(10,0,0));
	auto rplustranslation = r * glm::translate(glm::mat4(), glm::vec3(10, 10, 10));

	objMat = t * r;
	glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "objMat"), 1, GL_FALSE, glm::value_ptr(objMat));
	glUniform4f(glGetUniformLocation(cubeProgram, "color"), (float)sin(ImGui::GetTime()) * 0.5f, 1.f, 1.f, 0.f);
	glDrawElements(GL_TRIANGLE_STRIP, numVerts, GL_UNSIGNED_BYTE, 0);

	glUseProgram(0);
	glBindVertexArray(0);
	glDisable(GL_PRIMITIVE_RESTART);
}
}

/////////////////////////////////////////////////

std::vector<Object*> objects;



BillboardObject* point;

//Castle

MeshObject* pared;
MeshObject* Shiba;
MeshObject* ShibaOutline;
MeshObject* RacingTrack;
BillboardObject* RacingTrackBillboards;

MeshObject* trophy;
MeshObject* wheel;
MeshObject* table;

MeshObject* Camaro;

Spline spline; 


Transform cameraTransform;
Camera* currentCamera;
Camera mainCamera;
Camera mirrorCam;
Camera carCamera;


GLuint fboId;
GLuint fbo_tex;

void SetupFBO()
{
	glGenFramebuffers(1, &fboId);

	glGenTextures(1, &fbo_tex);
	glBindTexture(GL_TEXTURE_2D, fbo_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, dolly.screenWidth, dolly.screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glBindFramebuffer(GL_FRAMEBUFFER, fboId);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_tex, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void DrawFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fboId);
	glClearColor(1.f, 1.f, 1.f, 1.f);
	glViewport(0, 0, dolly.screenWidth, dolly.screenHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

}

void EndDrawFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, dolly.screenWidth, dolly.screenHeight);
	glEnable(GL_STENCIL_TEST);


}

void GLinit(int width, int height) {
	glViewport(0, 0, width, height);
	glClearColor(0.2f, 0.2f, 0.2f, 1.f);
	glClearDepth(1.f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_STENCIL_TEST);


	RV::_projection = glm::perspective(RV::FOV, (float)width / (float)height, RV::zNear, RV::zFar);

	Axis::setupAxis();
	OBJLoading::init();
	SetupFBO();

	Material shibaMat{ "shader.vert", "phongColor.frag" };
	Shiba = new MeshObject(shibaMat, "shiba.obj", "default_Base_Color.png", "Shiba");
	Shiba->Start();

	Shiba->transform.position = glm::vec3(0, 5, 10);
	Shiba->color = glm::vec4(1, 1, 1, 1);


	Material shibaMat2{ "shader.vert", "phongColor.frag" };
	ShibaOutline = new MeshObject(shibaMat2, "shiba.obj",  "ShibaOutline");
	ShibaOutline->Start();

	ShibaOutline->transform.position = glm::vec3(0, 5, 10);
	ShibaOutline->transform.scale = glm::vec3(1.3f, 1.3f, 1.3f);
	ShibaOutline->color = glm::vec4(0, 0, 0, 0);



	Material racingTrackMat{ "shader.vert", "phongColor.frag" };
	RacingTrack = new MeshObject(racingTrackMat, "racingTrack/track.obj", "racingTrack/PaletaMarioKart.png", "Track");
	RacingTrack->Start();

	RacingTrack->transform.position = glm::vec3(0, 0, 0);
	RacingTrack->transform.scale = glm::vec3(5, 5, 5);
	RacingTrack->color = glm::vec4(1, 1, 1, 1);


	Material billboardsMat{ "shaders/billboard.vert","shaders/billboard.frag", "shaders/billboard.geom"};
	
	RacingTrackBillboards = new BillboardObject(billboardsMat, "racingTrack/trackBillboards.obj", "AC_Tree.png", "Track Billboard");
	RacingTrackBillboards->Start();

	RacingTrackBillboards->transform.position = glm::vec3(0, 0, 0);
	RacingTrackBillboards->transform.scale = glm::vec3(5, 5, 5);
	RacingTrackBillboards->color = glm::vec4(1, 1, 1, 1);


	Material trophyMat{ "shader.vert", "phongColor.frag" };
	trophy = new MeshObject(trophyMat, "racingTrack/trophy.obj", "racingTrack/trophy.jpg", "Trophy");
	trophy->Start();

	trophy->transform.position = glm::vec3(0, 0, -8.f);
	trophy->transform.scale = glm::vec3(3, 3, 3);
	trophy->color = glm::vec4(1, 1, 1, 1);

	Material camaroMat{ "shader.vert", "phongColor.frag" };
	Camaro = new MeshObject(trophyMat, "racingTrack/Camaro.obj", "racingTrack/Camaro/Camaro_AlbedoTransparency_alt.png", "Camaro");
	Camaro->Start();

	Camaro->transform.position = glm::vec3(0, 0, -1.7f);
	Camaro->transform.scale = glm::vec3(0.01f, 0.01f, 0.01f);
	Camaro->color = glm::vec4(1, 1, 1, 1);

	//Material wheelMat{ "shader.vert", "phongColor.frag" };
	//wheel = new MeshObject(wheelMat, "racingTrack/wheel.obj", "racingTrack/wheel.png", "Wheel");
	//wheel->Start();

	//wheel->transform.position = glm::vec3(0, 0, 0);
	//wheel->color = glm::vec4(1, 1, 1, 1);

	//Material tableMat{ "shader.vert", "phongColor.frag" };
	//table = new MeshObject(tableMat, "racingTrack/table.obj",  "Table");
	//table->Start();

	//table->transform.position = glm::vec3(0, 0, 0);
	//table->color = glm::vec4(1, 1, 1, 1);





	Material phongMat2 { "shader.vert", "phongColor.frag" };
	pared = new MeshObject(phongMat2, "peachCastle/pared.obj", "peachCastle/49254A53_c.png", "Pared");
	pared->Start();
	dynamic_cast<Mesh::Mesh*>(&pared->Mesh())->SetOutsideTexture(fbo_tex);

	pared->transform.position = glm::vec3(0, 0, 0);
	pared->transform.scale = glm::vec3(5, 5, 5);
	pared->color = glm::vec4(1, 1, 1, 1);


	currentCamera = &mainCamera;

	cameraTransform.position = glm::vec3(0, -5, -20);
	mirrorCam.GetTransform()->position = glm::vec3(0, -5, -20);

	objects.push_back(RacingTrack);
	objects.push_back(RacingTrackBillboards);
	//objects.push_back(wheel);
	//objects.push_back(table);
	objects.push_back(trophy);
	objects.push_back(Camaro);


}

void GLcleanup() {
	Axis::cleanupAxis();
	OBJLoading::cleanup();

	for (auto object : objects)
	{
		object->Cleanup();
	}
	//peachCastleMesh->Cleanup();
	objects.clear();
	//pared->Cleanup();
	
}

bool carCameraActive = false;


void UpdateDollyEffect(float dt, int width, int height)
{

	if(dolly.currentFov <= dolly.endingFOV)
	{
		dolly.currentFov += dt;

		RV::_projection = glm::perspective(glm::radians(dolly.currentFov), (float)width / (float)height, RV::zNear, RV::zFar);
		float transformer = glm::abs(dolly.height / 2 * glm::tan(dolly.currentFov/2));
		std::cout << transformer << "\n";
	}
}

void UpdateTransformMatrix()
{
	cameraTransform = *currentCamera->GetTransform();

	RV::_modelView = glm::translate(RV::_modelView, glm::vec3(cameraTransform.position.x, cameraTransform.position.y, cameraTransform.position.z));
	RV::_modelView = glm::rotate(RV::_modelView, cameraTransform.rotation.x, glm::vec3(1.f, 0.f, 0.f));
	RV::_modelView = glm::rotate(RV::_modelView, cameraTransform.rotation.y, glm::vec3(0.f, 1.f, 0.f));

	RV::_MVP = RV::_projection * RV::_modelView;
}

float circle = 0.0f;
void GLupdate(float dt)
{
	RV::_modelView = glm::mat4(1.f);

	mainCamera.GetTransform()->position = glm::vec3{ RV::panv[0],  RV::panv[1] ,RV::panv[2] };
	mainCamera.GetTransform()->rotation = glm::vec3(RV::rota[1], RV::rota[0], 0);

	circle += dt;
	if(circle >= 23.f)
	{
		circle = 0.f;
	}

	Camaro->transform.position = spline.GetSplinePoint(circle);
	auto splineGradient = spline.GetSplineGradient(circle);
	float camaroAngle = atan2(-splineGradient.z, splineGradient.x);
	Camaro->transform.rotation = glm::vec3(0, 90 + camaroAngle, 0);
}	
	




void GLrender(float dt) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	time_t currentTime = SDL_GetTicks() / 1000;
	const GLfloat color[] = { 0.231373f,  0.439216f,0.501961f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, color);
	glStencilMask(0x00);


	Axis::drawAxis();
	
	currentCamera = &mirrorCam;
	UpdateTransformMatrix();
	
	DrawFBO();



	for (auto object : objects)
	{
		object->Render();
	}

	EndDrawFBO();
	//Switch camera to main

	if (carCameraActive)
	{
		currentCamera = &carCamera;
	}
	else
	{
		currentCamera = &mainCamera;
	}
	UpdateTransformMatrix();

	for (auto object : objects)
	{
		object->Render();
	}
	//glBindTexture(GL_TEXTURE_2D, fbo_tex);
	//pared->Render();
	//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	//glStencilFunc(GL_ALWAYS, 1, 0xFF); // all fragments should pass the stencil test
	//glStencilMask(0xFF);

	//
	//Shiba->Render();
	////glBindTexture(GL_TEXTURE_2D, fbo_tex);


	//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	//glStencilMask(0x00); // disable writing to the stencil buffer
	//glDepthMask(false);

	//ShibaOutline->Render();

	//glStencilMask(0xFF);
	//glStencilFunc(GL_ALWAYS, 1, 0xFF);
	//glDepthMask(true);

	//peachCastleMesh->Render();
	ImGui::Render();
}

void GUI() {
	bool show = true;
	ImGui::Begin("Physics Parameters", &show, 0);

	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::Dummy(ImVec2(0, 30));

		if(ImGui::CollapsingHeader("Directional Light"))
		{
			ImGui::DragFloat3("Direction", &Lighting::Instance()->directionalLight.direction[0]);
			ImGui::ColorEdit4("Light Color", &Lighting::Instance()->directionalLight.color[0]);

			ImGui::Dummy(ImVec2(0, 30));

			ImGui::ColorEdit4("Ambient Color", &Lighting::Instance()->directionalLight.ambientColor[0]);
			ImGui::DragFloat("Ambient Strength", &Lighting::Instance()->directionalLight.ambientStrength, 0.1f, 0, 1);
		}



		ImGui::Dummy(ImVec2(0, 30));



		if(ImGui::CollapsingHeader("Point Lights"))
		{
	
			for (int i = 0; i < Lighting::Instance()->GetPointLights().size(); ++i)
			{
				std::string s("Point Light " + std::to_string(i));

				ImGui::Dummy(ImVec2(0, 5));

				if(ImGui::TreeNode(s.c_str()))
				{
					ImGui::Checkbox(std::string("Is Active " + std::to_string(i)).c_str(), &Lighting::Instance()->pointLights[i].isActive);
					ImGui::DragFloat3(std::string("Position " + std::to_string(i)).c_str(), &Lighting::Instance()->pointLights[i].position[0],0.05f);
					ImGui::DragFloat("Intensity", &Lighting::Instance()->pointLights[i].strength, 0.1f, 0, 1000);
					ImGui::ColorEdit4(std::string("Light Color " + std::to_string(i)).c_str(), &Lighting::Instance()->pointLights[i].color[0]);

					ImGui::TreePop();

				}
			}

			 
		}
		ImGui::Dummy(ImVec2(0, 30));

		if (ImGui::CollapsingHeader("Spot Lights"))
		{

			for (int i = 0; i < Lighting::Instance()->GetSpotLights().size(); ++i)
			{
				std::string s("Spot Light " + std::to_string(i));

				ImGui::Dummy(ImVec2(0, 5));

				if (ImGui::TreeNode(s.c_str()))
				{
					ImGui::Checkbox(std::string("Is Active " + std::to_string(i)).c_str(), &Lighting::Instance()->spotLights[i].isActive);
					ImGui::DragFloat3(std::string("Position " + std::to_string(i)).c_str(), &Lighting::Instance()->spotLights[i].position[0]);
					ImGui::DragFloat3(std::string("Direction " + std::to_string(i)).c_str(), &Lighting::Instance()->spotLights[i].direction[0]);
					ImGui::DragFloat("Intensity", &Lighting::Instance()->spotLights[i].strength, 0.1f, 0, 1000);
					ImGui::DragFloat("Inner Cutoff", &Lighting::Instance()->spotLights[i].cutOff, 0.1f, 0, 180);
					ImGui::DragFloat("Outer Cutoff", &Lighting::Instance()->spotLights[i].outerCutoff, 0.1f, Lighting::Instance()->spotLights[i].cutOff, 180);
					ImGui::ColorEdit4(std::string("Light Color " + std::to_string(i)).c_str(), &Lighting::Instance()->spotLights[i].color[0]);


					ImGui::TreePop();

				}
			}


		}

		


	}
	

	ImGui::End();

	ImGui::Begin("Object Hierarchy");
	{
		for (auto object : objects)
		{
			ImGui::Dummy(ImVec2(0, 5));

			if (ImGui::TreeNode(object->name.c_str()))
			{
				ImGui::DragFloat3(std::string("Position " + object->name).c_str(),  &dynamic_cast<MeshObject*>(object)->transform.position[0], 0.05f);
				ImGui::DragFloat3(std::string("Rotation " + object->name).c_str(),  &dynamic_cast<MeshObject*>(object)->transform.rotation[0], 0.05f);
				ImGui::DragFloat3(std::string("Scale " + object->name).c_str(),  &dynamic_cast<MeshObject*>(object)->transform.scale[0], 0.05f);
				ImGui::ColorEdit4(std::string("Color " + object->name).c_str(), &dynamic_cast<MeshObject*>(object)->color[0]);

				if (ImGui::TreeNode("Material properties"))
				{
					ImGui::Text("Specular Highlights");
					ImGui::SliderFloat("Specular Strength", &dynamic_cast<MeshObject*>(object)->Mesh().specularStrength, 0, 1);
					ImGui::SliderInt("Specular Intensity", &dynamic_cast<MeshObject*>(object)->Mesh().specularIntensity, 16, 128);
					ImGui::TreePop();
				}

				ImGui::TreePop();

			}
		}
	}

	ImGui::End();




	bool show_test_window = false;
	if (show_test_window) {
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
		ImGui::ShowTestWindow(&show_test_window);
	}
}