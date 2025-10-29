
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
