#include "Spline.h"
#include <glm/glm.hpp>

Spline::Spline() 
{
	points.resize(23);
	points[0] = { 9.543756f, 0.081264f, -0.047095f };
	points[1] = { 9.543756f, 0.081264f, 1.397953f };
	points[2] = { 9.005232f, 0.081264f, -2.138164f };
	points[3] = { 9.533974f, 0.081264f, -0.984760f };
	points[4] = { 8.256747f, 0.081264f, -2.927343f };
	points[5] = { -4.668965f, 0.081264f, -10.161104f };
	points[6] = { -6.167093f, 0.081264f, -10.867343f };
	points[7] = { -8.099257f, 0.081264f, -10.653163f };
	points[8] = { -9.644037f, 0.081264f, -9.193027f };
	points[9] = { -9.559392f, 0.081264f, 3.588449f };
	points[10] = { -9.009196f, 0.081264f, 4.583035f };
	points[11] = { -7.951127f, 0.081264f, 4.963940f };
	points[12] = { -1.518065f, 0.081264f, 1.789732f };
	points[13] = { -0.184898f, 0.081264f, 1.535795f };
	points[14] = { 0.936656f, 0.081264f, 1.937861f };
	points[15] = { 2.777696f, 0.081264f, 4.942779f };
	points[16] = { 4.936157f, 0.081264f, 8.603699f };
	points[17] = { 6.269324f, 0.081264f, 9.640607f };
	points[18] = { 7.792944f, 0.081264f, 9.492477f };
	points[19] = { 9.168434f, 0.081264f, 8.519053f };
	points[20] = { 9.401210f, 0.081264f, 7.037756f };
	points[21] = { 9.507017f, 0.081264f, 4.667681f };
	points[22] = { 9.546220f, 0.081264f, -0.507159f };
	for (int i = 0; i < points.size(); ++i)
	{
		points[i] *= 5;
	}
}

glm::vec3 Spline::GetSplinePoint(float t)
{
	glm::vec3 point;
	int p0, p1, p2, p3;

	p1 = (int)t;
	p2 = (p1 + 1) % points.size();
	p3 = (p2 + 1) % points.size();
	p0 = p1 >= 1 ? p1 - 1 : points.size() - 1;

	t = t - (int)t;

	float tt = glm::pow(t,2);
	float ttt = glm::pow(t, 3);

	float q1 = -ttt + 2.0f * tt - t;
	float q2 = 3.0f * ttt - 5.0f * tt + 2.0f;
	float q3 = -3.0f * ttt + 4.0f * tt + t;
	float q4 = ttt - tt;

	point.x = 0.5f * (points[p0].x * q1 + points[p1].x * q2 + points[p2].x * q3 + points[p3].x * q4);
	point.y = 0.5f * (points[p0].y * q1 + points[p1].y * q2 + points[p2].y * q3 + points[p3].y * q4);
	point.z = 0.5f * (points[p0].z * q1 + points[p1].z * q2 + points[p2].z * q3 + points[p3].z * q4);


	return point;

}

glm::vec3 Spline::GetSplineGradient(float t)
{

	glm::vec3 point;
	int p0, p1, p2, p3;

	p1 = (int)t;
	p2 = (p1 + 1) % points.size();
	p3 = (p2 + 1) % points.size();
	p0 = p1 >= 1 ? p1 - 1 : points.size() - 1;

	t = t - (int)t;

	float tt = glm::pow(t, 2);
	float ttt = glm::pow(t, 3);

	float q1 = -3.0f * tt + 4.0f * t - 1;
	float q2 = 9.0f * tt - 10.0f * t;
	float q3 = -9.0f * tt + 8.0f * t + 1.0f;
	float q4 = 3.0f * tt - 2.0f * t;

	point.x = 0.5f * (points[p0].x * q1 + points[p1].x * q2 + points[p2].x * q3 + points[p3].x * q4);
	point.y = 0.5f * (points[p0].y * q1 + points[p1].y * q2 + points[p2].y * q3 + points[p3].y * q4);
	point.z = 0.5f * (points[p0].z * q1 + points[p1].z * q2 + points[p2].z * q3 + points[p3].z * q4);


	return point;
}
