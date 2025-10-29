
#ifndef MAIN_TOOLS_H
# define MAIN_TOOLS_H

# include <time.h>
# include "SDL.h"

int	figure_the_delay(clock_t start, clock_t end);
void ClearWindow();
void WindowSwap(SDL_Window *window);
void SetFrameTime(int used);

#endif
