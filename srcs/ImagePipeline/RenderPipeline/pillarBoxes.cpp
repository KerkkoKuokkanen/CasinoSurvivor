
#include "sprite.h"
#include "screen.h"
#include "commonTools.h"
#include "Textures.h"

#define ASPECT_RATIO 1.77778f

static bool boxesSet = false;
static float widthRemoval = 0.0f;
static float heightRemoval = 0.0f;
static unsigned int screenFrameRate = 0;

static int vpx = 0;
static int vpy = 0;
static int vpw = 0;
static int vph = 0;

static void SetOwnLetterBoxes()
{
	int width = __currentScreenWidth;
	int height = __currentScreenHeight;
	int targetHeight = rounding((float)width / ASPECT_RATIO);
	int removedPixels = height - targetHeight;
	removedPixels = removedPixels / 2.0f;
	float scale = 2.0f / (float)height;
	float poisitioning = scale * removedPixels;
	heightRemoval = poisitioning;
	widthRemoval = 0.0f;
}

static void SetOwnPillarBoxes()
{
	int width = __currentScreenWidth;
	int height = __currentScreenHeight;
	int targetWidth = rounding((float)height * ASPECT_RATIO);
	int removedPixels = width - targetWidth;
	removedPixels = removedPixels / 2;
	float scale = 2.0f / (float)width;
	float poisitioning = scale * removedPixels;
	widthRemoval = poisitioning;
	heightRemoval = 0.0f;
}

void UpdateGameViewport()
{
	float screenWidth = (float)__currentScreenWidth;
	float screenHeight = (float)__currentScreenHeight;
	float screenAspectRatio = screenWidth / screenHeight;

	// The final pixel coordinates and size for glViewport
	int viewportX = 0;
	int viewportY = 0;
	int viewportWidth = (int)screenWidth;
	int viewportHeight = (int)screenHeight;

	if (screenAspectRatio > ASPECT_RATIO) {
		// --- SCENARIO 1: PILLARBOX (Screen is WIDER than 16:9) ---
		// We constrain by height.
		viewportWidth = rounding(screenHeight * ASPECT_RATIO);
		viewportX = (rounding(screenWidth) - viewportWidth) / 2;

	} else if (screenAspectRatio < ASPECT_RATIO) {
		// --- SCENARIO 2: LETTERBOX (Screen is TALLER than 16:9) ---
		// We constrain by width.
		viewportHeight = rounding(screenWidth / ASPECT_RATIO);
		viewportY = (rounding(screenHeight) - viewportHeight) / 2;
	}
	vpx = viewportX;
	vpy = viewportY;
	vpw = viewportWidth;
	vph = viewportHeight;
	glViewport(viewportX, viewportY, viewportWidth, viewportHeight);
}

void SetViewPort()
{
	glViewport(vpx, vpy, vpw, vph);
}

void SetPillarBoxes()
{
	UpdateGameViewport();
	if (!__forceAspectRatio)
		return ;
	int width = __currentScreenWidth;
	int height = __currentScreenHeight;
	float ratio = (float)width / (float)height;
	if (FAlmostEqual(ASPECT_RATIO, ratio, 0.0001f))
		return ;
	if (ratio < ASPECT_RATIO)
		SetOwnLetterBoxes();
	else
		SetOwnPillarBoxes();
}

void ClearPillarBoxes()
{
	widthRemoval = 0.0f;
	heightRemoval = 0.0f;
}

float GetHeightMinus()
{
	return (heightRemoval);
}

float GetWidthMinus()
{
	return (widthRemoval);
}

void SetFrameRate(unsigned int frameRate)
{
	screenFrameRate = frameRate;
}

unsigned int GetFrameRate()
{
	return (screenFrameRate);
}
