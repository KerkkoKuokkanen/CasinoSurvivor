
#include "mainTools.h"
#include "pillarBoxes.h"
#include <OpenGL/gl3.h>

//DO NOT CHANGE HERE, USE THE FUNCTION
static double usedFrame = 0.5;

void ClearWindow()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void WindowSwap(SDL_Window *window)
{
	SDL_GL_SwapWindow(window);
}

Uint32 figure_the_delay(Uint64 start, Uint64 freq)
{
	Uint64 frameWorkTicks = SDL_GetPerformanceCounter() - start;
	double frameWorkTimeSeconds = (double)frameWorkTicks / (double)freq;
	double remainingTimeSeconds = usedFrame - frameWorkTimeSeconds;
	if (remainingTimeSeconds > 0.0)
	{
		Uint32 delayMs = (Uint32)(remainingTimeSeconds * 1000.0);
		if (delayMs > 1)
			return (delayMs - 1);
		return (delayMs);
	}
	return (0);
}

void SetFrameTime(int fps)
{
	usedFrame = 1.0 / (double)fps;
}
