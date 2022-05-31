#pragma once
#include <glm/glm.hpp>

struct DirectionalLight
{
	glm::vec3 direction;

	glm::vec4 color;
	glm::vec4 ambientColor;
	float ambientStrength;

	DirectionalLight() : direction(2,1,0), color(0.9f,0.9f,0.9f,1.f),
						ambientColor(0.75f, 0.75f, 0.75f, 1.f), ambientStrength(0.7f)
	{
	}

};

struct PointLight 
{

	bool isActive = false; 
	glm::vec3 position;
	float strength;

	glm::vec4 color;
	glm::vec4 ambientColor;
	float ambientStrength;

	float constant = 1.0f;
	float linear = 0.7f;
	float quadratic = 1.8f;
};

struct SpotLight
{

	bool isActive = false;
	glm::vec3 position;
	glm::vec3 direction;
	float strength;

	float cutOff;
	float outerCutoff;

	glm::vec4 color; 
	glm::vec4 ambientColor;
	float ambientStrength;

};