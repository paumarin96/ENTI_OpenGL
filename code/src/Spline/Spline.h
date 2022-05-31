#pragma once
#include <glm/detail/type_vec.hpp>
#include "../Mesh/MeshLoader.h"
#include "vector"


class Spline
{

private:
	std::vector<glm::vec3> points;

public:

	Spline();
	glm::vec3 GetSplinePoint(float t);
	glm::vec3 GetSplineGradient(float t);


};
