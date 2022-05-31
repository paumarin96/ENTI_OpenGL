#pragma once
#include "..\Transform.h"

class Camera
{



private:

	Transform transform;
public:
	Transform* GetTransform() 
	{
		return &transform;
	}

	float GetFov() 
	{
		return fov;
	}

private:
	float fov;


};

