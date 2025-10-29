
#ifndef MOUSE_OVER_H
# define MOUSE_OVER_H

# include <SDL2/SDL.h>
# include "box.h"

bool MouseOverSprite(SDL_Surface *sur, t_Box dest);
bool MouseOverBoundingBox(t_BoundingB &box);
bool MouseOverBox(t_Box &box);

#endif
