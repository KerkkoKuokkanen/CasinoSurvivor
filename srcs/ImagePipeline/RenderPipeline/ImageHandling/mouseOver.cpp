
#include "mouseOver.h"
#include "mouse.h"
#include "screen.h"

bool MouseOverSprite(SDL_Surface *sur, t_Box dest)
{
	return (false);
}

static bool CheckInsideShape(t_Point up, t_Point left, t_Point down, t_Point right, t_Point target)
{
	t_Point vec1 = {left.x - up.x, left.y - up.y};
	t_Point vec2 = {down.x - left.x, down.y - left.y};
	t_Point vec3 = {right.x - down.x, right.y - down.y};
	t_Point vec4 = {up.x - right.x, up.y - right.y};
	t_Point vp1 = {target.x - up.x, target.y - up.y};
	t_Point vp2 = {target.x - left.x, target.y - left.y};
	t_Point vp3 = {target.x - down.x, target.y - down.y};
	t_Point vp4 = {target.x - right.x, target.y - right.y};
	float cross1 = vec1.x * vp1.y - vec1.y * vp1.x;
	float cross2 = vec2.x * vp2.y - vec2.y * vp2.x;
	float cross3 = vec3.x * vp3.y - vec3.y * vp3.x;
	float cross4 = vec4.x * vp4.y - vec4.y * vp4.x;
	if (cross1 * cross2 > 0.0f && cross1 * cross3 > 0.0f && cross1 * cross4 > 0.0f)
		return (true);
	return (false);
}

bool MouseOverBoundingBox(t_BoundingB &box)
{
	t_Point xy =  GetMouseXY();
	if (CheckInsideShape(box.leftTop, box.leftBottom, box.rightBottom, box.rightTop, xy))
		return (true);
	return (false);
}

bool MouseOverBox(t_Box &box)
{
	t_Point xy =  GetMouseXY();
	if (box.x < xy.x && (box.x + box.w) > xy.x)
	{
		if (box.y < xy.y && (box.y + box.h) > xy.y)
			return (true);
	}
	return (false);
}
