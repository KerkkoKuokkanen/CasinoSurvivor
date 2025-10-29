
#include "mainTools.h"
#include "pillarBoxes.h"
#include <OpenGL/gl3.h>

//DO NOT CHANGE HERE, USE THE FUNCTION
static int usedFrame = 17;

void ClearWindow()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void WindowSwap(SDL_Window *window)
{
	SDL_GL_SwapWindow(window);
}

int	figure_the_delay(clock_t start, clock_t end)
{
	double	time;
	int		ret;
	time = 0.0;
	time += (double)(end - start) / CLOCKS_PER_SEC;
	ret = usedFrame - (int)(time * 1000.0f);
	if (ret < 0)
		return (0);
	return (ret);
}

void SetFrameTime(int used)
{
	usedFrame = used;
}
