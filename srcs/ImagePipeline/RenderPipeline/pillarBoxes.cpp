
#include "sprite.h"
#include "screen.h"
#include "commonTools.h"
#include "Textures.h"

#define ASPECT_RATIO 1.77778f

static unsigned int screenFrameRate = 0;

static int vpx = 0;
static int vpy = 0;
static int vpw = 0;
static int vph = 0;

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
}

void SetFrameRate(unsigned int frameRate)
{
	screenFrameRate = frameRate;
}

unsigned int GetFrameRate()
{
	return (screenFrameRate);
}
