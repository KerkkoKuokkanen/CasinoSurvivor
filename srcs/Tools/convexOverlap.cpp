
#include "convexOverlap.h"
#include <cmath>
#include <algorithm>

static t_BoundingB screen = {{-1.0f, 1.0f}, {1.0f, 1.0f}, {-1.0f, -1.0f}, {1.0f, -1.0f}};

static float projectPoint(const t_Point& point, const t_Point& axis)
{
	return (point.x * axis.x + point.y * axis.y);
}

static bool overlapOnAxis(const t_BoundingB& box1, const t_BoundingB& box2, const t_Point& axis)
{
	float box1Min = projectPoint(box1.leftTop, axis);
	float box1Max = box1Min;

	float projections[3] = {
		projectPoint(box1.rightTop, axis),
		projectPoint(box1.leftBottom, axis),
		projectPoint(box1.rightBottom, axis)
	};
	for (float proj : projections)
	{
		box1Min = std::min(box1Min, proj);
		box1Max = std::max(box1Max, proj);
	}

	float box2Min = projectPoint(box2.leftTop, axis);
	float box2Max = box2Min;

	float projections2[3] = {
		projectPoint(box2.rightTop, axis),
		projectPoint(box2.leftBottom, axis),
		projectPoint(box2.rightBottom, axis)
	};
	for (float proj : projections2)
	{
		box2Min = std::min(box2Min, proj);
		box2Max = std::max(box2Max, proj);
	}

	return (box1Max >= box2Min && box2Max >= box1Min);
}

bool ConvexOverlap(t_BoundingB &rect1, t_BoundingB &rect2)
{
	t_Point edges[4] = {
		{ rect2.rightTop.x - rect2.leftTop.x, rect2.rightTop.y - rect2.leftTop.y },
		{ rect2.rightBottom.x - rect2.rightTop.x, rect2.rightBottom.y - rect2.rightTop.y },
		{ rect1.rightTop.x - rect1.leftTop.x, rect1.rightTop.y - rect1.leftTop.y },
		{ rect1.rightBottom.x - rect1.rightTop.x, rect1.rightBottom.y - rect1.rightTop.y }
	};

	t_Point axes[4];
	for (int i = 0; i < 4; ++i)
	{
		t_Point normal = { -edges[i].y, edges[i].x };
		float length = std::sqrt(normal.x * normal.x + normal.y * normal.y);
		axes[i] = { normal.x / length, normal.y / length };
	}

	for (const auto& axis : axes)
	{
		float length = std::sqrt(axis.x * axis.x + axis.y * axis.y);
		t_Point normalizedAxis = { axis.x / length, axis.y / length };
		if (!overlapOnAxis(rect2, rect1, normalizedAxis))
			return false;
	}
	return true;
}

bool ReactangleScreenOverlap(t_BoundingB& rect)
{
	return (ConvexOverlap(rect, screen));
}

static bool overlapOnAxis2(const t_BoundingB& box1, const t_BoundingB& box2, const t_Point& axis, float& penetration)
{
	float box1Min = projectPoint(box1.leftTop, axis);
	float box1Max = box1Min;

	float projections[3] = {
		projectPoint(box1.rightTop, axis),
		projectPoint(box1.leftBottom, axis),
		projectPoint(box1.rightBottom, axis)
	};
	for (float proj : projections)
	{
		box1Min = std::min(box1Min, proj);
		box1Max = std::max(box1Max, proj);
	}

	float box2Min = projectPoint(box2.leftTop, axis);
	float box2Max = box2Min;

	float projections2[3] = {
		projectPoint(box2.rightTop, axis),
		projectPoint(box2.leftBottom, axis),
		projectPoint(box2.rightBottom, axis)
	};
	for (float proj : projections2)
	{
		box2Min = std::min(box2Min, proj);
		box2Max = std::max(box2Max, proj);
	}

	if (box1Max < box2Min || box2Max < box1Min)
		return false;

	// Calculate overlap
	penetration = std::min(box1Max, box2Max) - std::max(box1Min, box2Min);
	return true;
}

bool ConvexOverlap2(t_BoundingB &rect1, t_BoundingB &rect2, float& minPenetrationDepth, t_Point& collisionNormal)
{
	t_Point edges[4] = {
		{ rect2.rightTop.x - rect2.leftTop.x, rect2.rightTop.y - rect2.leftTop.y },
		{ rect2.rightBottom.x - rect2.rightTop.x, rect2.rightBottom.y - rect2.rightTop.y },
		{ rect1.rightTop.x - rect1.leftTop.x, rect1.rightTop.y - rect1.leftTop.y },
		{ rect1.rightBottom.x - rect1.rightTop.x, rect1.rightBottom.y - rect1.rightTop.y }
	};

	t_Point axes[4];
	for (int i = 0; i < 4; ++i)
	{
		t_Point normal = { -edges[i].y, edges[i].x };
		float length = std::sqrt(normal.x * normal.x + normal.y * normal.y);
		axes[i] = { normal.x / length, normal.y / length };
	}

	minPenetrationDepth = std::numeric_limits<float>::infinity();
	collisionNormal = {0, 0};

	for (const auto& axis : axes)
	{
		float penetration;
		if (!overlapOnAxis2(rect1, rect2, axis, penetration))
			return false;

		if (penetration < minPenetrationDepth)
		{
			minPenetrationDepth = penetration;
			collisionNormal = axis;
		}
	}
	return true;
}
