
#ifndef MAIN_TOOLS_H
# define MAIN_TOOLS_H

# include <time.h>
# include "SDL.h"

Uint32 figure_the_delay(Uint64 start, Uint64 freq);
void ClearWindow();
void WindowSwap(SDL_Window *window);
void SetFrameTime(int used);

#endif
