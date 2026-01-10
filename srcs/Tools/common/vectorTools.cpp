
#include "box.h"
#include <math.h>

float VectorMagnitude(t_Point vec)
{
	float mag = sqrt(vec.x * vec.x + vec.y * vec.y);
	return (mag);
}

float VectorCross(t_Point a, t_Point b)
{
	return a.x * b.y - a.y * b.x;
}

t_Point VectorPerp(t_Point v)
{
	return {-v.y, v.x};
}

t_Point VectorRotate(t_Point vec, float angle)
{
	float rotatedX = vec.x * cos(angle) - vec.y * sin(angle);
	float rotatedY = vec.x * sin(angle) + vec.y * cos(angle);
	return (t_Point{rotatedX, rotatedY});
}

t_Point VectorNormalize(t_Point vec)
{
	float mag = VectorMagnitude(vec);
	if (mag == 0.0f)
		return {0.0f, 0.0f};
	vec.x /= mag;
	vec.y /= mag;
	return (vec);
}

t_Point VectorScalar(t_Point vec, float multi)
{
	vec.x = vec.x * multi;
	vec.y = vec.y * multi;
	return (vec);
}

t_Point VectorAdd(t_Point vec1, t_Point vec2)
{
	float x = vec1.x + vec2.x;
	float y = vec1.y + vec2.y;
	return ((t_Point){x, y});
}

t_Point VectorSubtract(t_Point vec1, t_Point vec2)
{
	float x = vec1.x - vec2.x;
	float y = vec1.y - vec2.y;
	return ((t_Point){x, y});
}

float VectorDotProduct(t_Point vec1, t_Point vec2)
{
	float ret = vec1.x * vec2.x + vec1.y * vec2.y;
	return (ret);
}

float VectorAngle(t_Point vec1, t_Point vec2)
{
	float dot = VectorDotProduct(vec1, vec2);
	float mag1 = VectorMagnitude(vec1);
	float mag2 = VectorMagnitude(vec2);
	return (acos(dot / (mag1 * mag2)));
}

#include <vector>

static std::vector<float> sinRand;
static std::vector<float> cosRand;

float RandCos()
{
	int r = rand() % 360;
	return (cosRand[r]);
}

float RandSin()
{
	int r = rand() % 360;
	return (sinRand[r]);
}

float GetSinAngle(unsigned int angle)
{
	if (angle >= 360)
		angle = angle % 360;
	return (sinRand[angle]);
}

float GetCosAngle(unsigned int angle)
{
	if (angle >= 360)
		angle = angle % 360;
	return (cosRand[angle]);
}

t_Point RandSinAndCos()
{
	int r = rand() % 360;
	t_Point ret = {sinRand[r], cosRand[r]};
	return (ret);
}

void InitRandSinCos()
{
	for (int i = 0; i < 360; ++i)
	{
		float angle = 2.0 * PI * i / 360;
		cosRand.push_back(std::cos(angle));
		sinRand.push_back(std::sin(angle));
	}
}
