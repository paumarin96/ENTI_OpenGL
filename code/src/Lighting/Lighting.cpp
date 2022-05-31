#include "Lighting.h"

Lighting* Lighting::_instance = nullptr;

Lighting::Lighting(): directionalLight(),
	                                            pointLights(),
	                                            spotLights()
{
	pointLights.push_back(PointLight{ true,glm::vec3(3, 0, 0), 1, glm::vec4(0, 0, 1, 1), glm::vec4(0, 0, 0, 1), 0.f, 1.0f, 0.7f, 1.8f });
	pointLights.push_back(PointLight{ false,glm::vec3(3, 0, 0), 1, glm::vec4(0, 0, 1, 1), glm::vec4(0, 0, 0, 1), 0.f, 1.0f, 0.7f, 1.8f });
	pointLights.push_back(PointLight{ false, glm::vec3(3, 0, 0), 1, glm::vec4(0, 0, 1, 1), glm::vec4(0, 0, 0, 1), 0.f, 1.0f, 0.7f, 1.8f });
	pointLights.push_back(PointLight{ false, glm::vec3(3, 0, 0), 1, glm::vec4(0, 0, 1, 1), glm::vec4(0, 0, 0, 1), 0.f, 1.0f, 0.7f, 1.8f });
	pointLights.push_back(PointLight{ false, glm::vec3(3, 0, 0), 1, glm::vec4(0, 0, 1, 1), glm::vec4(0, 0, 0, 1), 0.f, 1.0f, 0.7f, 1.8f });
	pointLights.push_back(PointLight{ false, glm::vec3(3, 0, 0), 1, glm::vec4(0, 0, 1, 1), glm::vec4(0, 0, 0, 1), 0.f, 1.0f, 0.7f, 1.8f });
	pointLights.push_back(PointLight{ false, glm::vec3(3, 0, 0), 1, glm::vec4(0, 0, 1, 1), glm::vec4(0, 0, 0, 1), 0.f, 1.0f, 0.7f, 1.8f });
	pointLights.push_back(PointLight{ false, glm::vec3(3, 0, 0), 1, glm::vec4(0, 0, 1, 1), glm::vec4(0, 0, 0, 1), 0.f, 1.0f, 0.7f, 1.8f });
	pointLights.push_back(PointLight{ false, glm::vec3(3, 0, 0), 1, glm::vec4(0, 0, 1, 1), glm::vec4(0, 0, 0, 1), 0.f, 1.0f, 0.7f, 1.8f });
	pointLights.push_back(PointLight{ false, glm::vec3(3, 0, 0), 1, glm::vec4(0, 0, 1, 1), glm::vec4(0, 0, 0, 1), 0.f, 1.0f, 0.7f, 1.8f });
	pointLights.push_back(PointLight{ false, glm::vec3(3, 0, 0), 1, glm::vec4(0, 0, 1, 1), glm::vec4(0, 0, 0, 1), 0.f, 1.0f, 0.7f, 1.8f });
	pointLights.push_back(PointLight{ false, glm::vec3(3, 0, 0), 1, glm::vec4(0, 0, 1, 1), glm::vec4(0, 0, 0, 1), 0.f, 1.0f, 0.7f, 1.8f });
	pointLights.push_back(PointLight{ false, glm::vec3(3, 0, 0), 1, glm::vec4(0, 0, 1, 1), glm::vec4(0, 0, 0, 1), 0.f, 1.0f, 0.7f, 1.8f });
	pointLights.push_back(PointLight{ false, glm::vec3(3, 0, 0), 1, glm::vec4(0, 0, 1, 1), glm::vec4(0, 0, 0, 1), 0.f, 1.0f, 0.7f, 1.8f });
	pointLights.push_back(PointLight{ false, glm::vec3(3, 0, 0), 1, glm::vec4(0, 0, 1, 1), glm::vec4(0, 0, 0, 1), 0.f, 1.0f, 0.7f, 1.8f });
	pointLights.push_back(PointLight{ false, glm::vec3(3, 0, 0), 1, glm::vec4(0, 0, 1, 1), glm::vec4(0, 0, 0, 1), 0.f, 1.0f, 0.7f, 1.8f });

	spotLights.push_back(SpotLight{ true,glm::vec3(3, 5, 0), glm::vec3(0, -1, 0), 1, glm::radians(15.f), glm::radians(20.f), glm::vec4(1.f, 0, 0, 1.f), glm::vec4(0, 0, 1.f, 1), 0.3f});
	spotLights.push_back(SpotLight{ false,glm::vec3(3, 5, 0), glm::vec3(0, -1, 0), 1, glm::radians(15.f), glm::radians(20.f), glm::vec4(1.f, 0, 0, 1.f), glm::vec4(0, 0, 1.f, 1), 0.3f});
	spotLights.push_back(SpotLight{ false,glm::vec3(3, 5, 0), glm::vec3(0, -1, 0), 1, glm::radians(15.f), glm::radians(20.f), glm::vec4(1.f, 0, 0, 1.f), glm::vec4(0, 0, 1.f, 1), 0.3f});
	spotLights.push_back(SpotLight{ false,glm::vec3(3, 5, 0), glm::vec3(0, -1, 0), 1, glm::radians(15.f), glm::radians(20.f), glm::vec4(1.f, 0, 0, 1.f), glm::vec4(0, 0, 1.f, 1), 0.3f});
	spotLights.push_back(SpotLight{ false,glm::vec3(3, 5, 0), glm::vec3(0, -1, 0), 1, glm::radians(15.f), glm::radians(20.f), glm::vec4(1.f, 0, 0, 1.f), glm::vec4(0, 0, 1.f, 1), 0.3f});
	spotLights.push_back(SpotLight{ false,glm::vec3(3, 5, 0), glm::vec3(0, -1, 0), 1, glm::radians(15.f), glm::radians(20.f), glm::vec4(1.f, 0, 0, 1.f), glm::vec4(0, 0, 1.f, 1), 0.3f});
	spotLights.push_back(SpotLight{ false,glm::vec3(3, 5, 0), glm::vec3(0, -1, 0), 1, glm::radians(15.f), glm::radians(20.f), glm::vec4(1.f, 0, 0, 1.f), glm::vec4(0, 0, 1.f, 1), 0.3f});
	spotLights.push_back(SpotLight{ false,glm::vec3(3, 5, 0), glm::vec3(0, -1, 0), 1, glm::radians(15.f), glm::radians(20.f), glm::vec4(1.f, 0, 0, 1.f), glm::vec4(0, 0, 1.f, 1), 0.3f});
	spotLights.push_back(SpotLight{ false,glm::vec3(3, 5, 0), glm::vec3(0, -1, 0), 1, glm::radians(15.f), glm::radians(20.f), glm::vec4(1.f, 0, 0, 1.f), glm::vec4(0, 0, 1.f, 1), 0.3f});
	spotLights.push_back(SpotLight{ false,glm::vec3(3, 5, 0), glm::vec3(0, -1, 0), 1, glm::radians(15.f), glm::radians(20.f), glm::vec4(1.f, 0, 0, 1.f), glm::vec4(0, 0, 1.f, 1), 0.3f});
	spotLights.push_back(SpotLight{ false,glm::vec3(3, 5, 0), glm::vec3(0, -1, 0), 1, glm::radians(15.f), glm::radians(20.f), glm::vec4(1.f, 0, 0, 1.f), glm::vec4(0, 0, 1.f, 1), 0.3f});
	spotLights.push_back(SpotLight{ false,glm::vec3(3, 5, 0), glm::vec3(0, -1, 0), 1, glm::radians(15.f), glm::radians(20.f), glm::vec4(1.f, 0, 0, 1.f), glm::vec4(0, 0, 1.f, 1), 0.3f});
	spotLights.push_back(SpotLight{ false,glm::vec3(3, 5, 0), glm::vec3(0, -1, 0), 1, glm::radians(15.f), glm::radians(20.f), glm::vec4(1.f, 0, 0, 1.f), glm::vec4(0, 0, 1.f, 1), 0.3f});
	spotLights.push_back(SpotLight{ false,glm::vec3(3, 5, 0), glm::vec3(0, -1, 0), 1, glm::radians(15.f), glm::radians(20.f), glm::vec4(1.f, 0, 0, 1.f), glm::vec4(0, 0, 1.f, 1), 0.3f});
	spotLights.push_back(SpotLight{ false,glm::vec3(3, 5, 0), glm::vec3(0, -1, 0), 1, glm::radians(15.f), glm::radians(20.f), glm::vec4(1.f, 0, 0, 1.f), glm::vec4(0, 0, 1.f, 1), 0.3f});
	spotLights.push_back(SpotLight{ false,glm::vec3(3, 5, 0), glm::vec3(0, -1, 0), 1, glm::radians(15.f), glm::radians(20.f), glm::vec4(1.f, 0, 0, 1.f), glm::vec4(0, 0, 1.f, 1), 0.3f});




}
