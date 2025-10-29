
#include "imageTransforms.h"
#include "pillarBoxes.h"
#include "screen.h"
#include <math.h>

#define DEFAULT_SCREEN_SPACE_WIDTH 10.0f
#define DEFAULT_SCREEN_SPACE_HEIGHT 10.0f

float __ScreenSpaceUsedWidth;
float __ScreenSpaceUsedHeight;

float __CameraX;
float __CameraY;

t_iPoint TransformToPixelCoordinates(float x, float y)
{
	x += __ScreenSpaceUsedWidth;
	y = abs(y - __ScreenSpaceUsedHeight);
	float scaleX = (float)__currentScreenWidth / (2.0f * __ScreenSpaceUsedWidth);
	float scaleY = (float)__currentScreenHeight / (2.0f * __ScreenSpaceUsedHeight);
	int posX = round(scaleX * x);
	int posY = round(scaleY * y);
	return (t_iPoint{posX, posY});
}

t_Point TransformCoordinateToScreenSpace(float x, float y)
{
	float widthUnit = 1.0f - GetWidthMinus();
	float heightUnit = 1.0f - GetHeightMinus();
	float widthScale = widthUnit / __ScreenSpaceUsedWidth;
	float heightScale = heightUnit / __ScreenSpaceUsedHeight;
	float wPosition = widthScale * x;
	float hPosition = heightScale * y;
	return (t_Point{wPosition, hPosition});
}

t_Point TransformCoordinateToScreenSpaceCamera(float x, float y)
{
	x = x - __CameraX;
	y = y - __CameraY;
	float widthUnit = 1.0f - GetWidthMinus();
	float heightUnit = 1.0f - GetHeightMinus();
	float widthScale = widthUnit / __ScreenSpaceUsedWidth;
	float heightScale = heightUnit / __ScreenSpaceUsedHeight;
	float wPosition = widthScale * x;
	float hPosition = heightScale * y;
	return (t_Point{wPosition, hPosition});
}

float TransformWidthToCameraSpace(float w)
{
	float scale = 10.0f / __ScreenSpaceUsedWidth;
	return (w *= scale);
}

float TransformHeightToCameraSpace(float h)
{
	float scale = 10.0f / __ScreenSpaceUsedHeight;
	return (h *= scale);
}

void SetScreenSpaceDimentions(float w, float h)
{
	__ScreenSpaceUsedWidth = w;
	__ScreenSpaceUsedHeight = h;
}

void SetCameraCoordinates(float x, float y)
{
	__CameraX = x;
	__CameraY = y;
}

void ResetImageTransformations()
{
	__ScreenSpaceUsedWidth = DEFAULT_SCREEN_SPACE_WIDTH;
	__ScreenSpaceUsedHeight = DEFAULT_SCREEN_SPACE_HEIGHT;
	__CameraX = 0.0f;
	__CameraY = 0.0f;
}
