#pragma once
#include <string>

#include "glm/glm.hpp"
#include "../Lighting/Lighting.h"


class Object
{
private: 

public:
	std::string name;
	Object(std::string name) : name(name){}
	virtual void Start() = 0;
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;
	virtual void Cleanup() = 0;


};

