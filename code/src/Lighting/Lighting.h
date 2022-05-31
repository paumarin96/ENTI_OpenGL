#pragma once
#include <vector>

#include "Light.h"
class Lighting
{
public:

	DirectionalLight directionalLight; 
	std::vector<PointLight> pointLights;
	std::vector<SpotLight> spotLights;


private:
	static Lighting* _instance;
public:


	Lighting();

	static Lighting* Instance()
	{
		if(_instance == nullptr)
		{
			_instance = new Lighting();
		}
		return _instance;
	}

	DirectionalLight* GetDirectionalLight()
	{
		return &directionalLight;
	}

	std::vector<PointLight>& GetPointLights()
	{
		return pointLights; 
	}

	std::vector<SpotLight>& GetSpotLights()
	{
		return spotLights;
	}

};

