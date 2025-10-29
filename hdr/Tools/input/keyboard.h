
#ifndef KEYBOARD_H
# define KEYBOARD_H

# include <SDL2/SDL.h>

void UpdateKeyInputs();
bool KeyPressed(int SDL_KeyCode);
bool KeyReleased(int SDL_KeyCode);
bool KeyHeld(int SDL_KeyCode);

#endif
