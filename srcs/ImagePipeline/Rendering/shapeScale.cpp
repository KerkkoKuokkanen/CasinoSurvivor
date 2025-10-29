
#include "shape.h"
#include "vectorTools.h"

static t_Point RotatePoint(t_Point xy, float centerX, float centerY, float cosTheta, float sinTheta)
{
	float xPos = xy.x - centerX;
	float yPos = xy.y - centerY;
	float xNew = xPos * cosTheta - yPos * sinTheta;
	float yNew = xPos * sinTheta + yPos * cosTheta;
	t_Point ret = {xNew + centerX, yNew + centerY};
	return ret;
}

static t_Point GetCenter(t_BoundingB &boundBox)
{
	float xCenter = (boundBox.leftBottom.x + boundBox.leftTop.x + boundBox.rightBottom.x + boundBox.rightTop.x) / 4.0f;
	float yCenter = (boundBox.leftBottom.y + boundBox.leftTop.y + boundBox.rightBottom.y + boundBox.rightTop.y) / 4.0f;
	return ((t_Point){xCenter, yCenter});
}

void GLShape::SetDirScaleDir(t_Point dir)
{
	scaleDirection = {dir.x, dir.y};
}

void GLShape::CalculateBBoxPosition(t_BoundingB &box)
{
	t_Point center = GetCenter(boundBox);
	float x = position.x + pivotPoint.x;
	float y = position.y + pivotPoint.y;
	float xAdd = x - center.x;
	float yAdd = y - center.y;
	box.leftBottom.x += xAdd;
	box.leftBottom.y += yAdd;
	box.leftTop.x += xAdd;
	box.leftTop.y += yAdd;
	box.rightBottom.x += xAdd;
	box.rightBottom.y += yAdd;
	box.rightTop.x += xAdd;
	box.rightTop.y += yAdd;
}

void GLShape::CalculateBBoxWidth(t_BoundingB &box)
{
	t_Point center = GetCenter(box);
	float ogW = abs(box.rightTop.x - center.x);
	float usedW = ogW * width;
	box.leftTop.x = center.x - usedW;
	box.leftBottom.x = center.x - usedW;
	box.rightTop.x = center.x + usedW;
	box.rightBottom.x = center.x + usedW;
}

void GLShape::CalculateBBoxHeight(t_BoundingB &box)
{
	t_Point center = GetCenter(box);
	float ogH = abs(box.rightTop.y - center.y);
	float usedH = ogH * height;
	box.leftTop.y = center.y + usedH;
	box.leftBottom.y = center.y - usedH;
	box.rightTop.y = center.y + usedH;
	box.rightBottom.y = center.y - usedH;
}

void GLShape::CalculateBBoxRotation(t_BoundingB &box)
{
	t_Point center = GetCenter(box);
	float cosA = std::cos(angle);
	float sinA = std::sin(angle);
	box.leftBottom =  RotatePoint(box.leftBottom, center.x, center.y, cosA, sinA);
	box.leftTop = RotatePoint(box.leftTop, center.x, center.y, cosA, sinA);
	box.rightBottom = RotatePoint(box.rightBottom, center.x, center.y, cosA, sinA);
	box.rightTop = RotatePoint(box.rightTop, center.x, center.y, cosA, sinA);
}

t_Point GLShape::CalculateBBoxDirScalePoint(t_Point pos, t_Point center, t_Point normal, float multi)
{
	t_Point p1 = VectorSubtract(pos, center);
	float dot = VectorDotProduct(p1, normal);
	t_Point scale = VectorScalar(normal, dot);
	t_Point projection = VectorAdd(center, scale);
	t_Point off = VectorSubtract(pos, projection);
	float dist = VectorMagnitude(off);
	if (dist < 0.00001f)
		return (pos);
	pos.x = projection.x + multi * off.x;
	pos.y = projection.y + multi * off.y;
	return (pos);
}

void GLShape::CalculateBBoxDirScale(t_BoundingB &box)
{
	t_Point center = GetCenter(box);
	t_Point dir = VectorNormalize(scaleDirection);
	t_Point normal = VectorNormalize({-dir.y, dir.x});
	float scaler = scaleDir;

	box.leftBottom = CalculateBBoxDirScalePoint(box.leftBottom, center, normal, scaler);
	box.leftTop = CalculateBBoxDirScalePoint(box.leftTop, center, normal, scaler);
	box.rightBottom = CalculateBBoxDirScalePoint(box.rightBottom, center, normal, scaler);
	box.rightTop = CalculateBBoxDirScalePoint(box.rightTop, center, normal, scaler);
}

void GLShape::CalculateBBoxPerpScale(t_BoundingB &box)
{
	t_Point center = GetCenter(box);
	t_Point dir = VectorNormalize(scaleDirection);
	t_Point normal = dir;
	float scaler = scalePerp;

	box.leftBottom = CalculateBBoxDirScalePoint(box.leftBottom, center, normal, scaler);
	box.leftTop = CalculateBBoxDirScalePoint(box.leftTop, center, normal, scaler);
	box.rightBottom = CalculateBBoxDirScalePoint(box.rightBottom, center, normal, scaler);
	box.rightTop = CalculateBBoxDirScalePoint(box.rightTop, center, normal, scaler);
}

void GLShape::UpdateBoundingBox()
{
	t_BoundingB calculated = boundBox;
	CalculateBBoxPosition(calculated);
	CalculateBBoxWidth(calculated);
	CalculateBBoxHeight(calculated);
	CalculateBBoxRotation(calculated);
	CalculateBBoxDirScale(calculated);
	CalculateBBoxPerpScale(calculated);
	rotatedBBox = calculated;
}

void GLShape::SetDirScaleWidth(float w)
{
	scaleDir = w;
	UpdateBoundingBox();
}


void GLShape::SetDirScaleHeight(float h)
{
	scalePerp = h;
	UpdateBoundingBox();
}

void GLShape::SetHeight(float h)
{
	height = h;
	UpdateBoundingBox();
}

void GLShape::SetWidth(float w)
{
	width = w;
	UpdateBoundingBox();
}
