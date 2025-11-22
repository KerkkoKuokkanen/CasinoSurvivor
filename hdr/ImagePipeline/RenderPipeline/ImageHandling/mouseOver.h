
#ifndef MOUSE_OVER_H
# define MOUSE_OVER_H

# include <SDL2/SDL.h>
# include "box.h"
# include "image.h"

bool MouseOverImage(Image *img);
bool MouseOverBoundingBox(t_BoundingB &box);
bool MouseOverBox(t_Box &box);

#endif
