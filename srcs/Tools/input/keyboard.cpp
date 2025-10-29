
#include "keyboard.h"

static bool currentKeyState[SDL_NUM_SCANCODES] = {0};
static bool previousKeyState[SDL_NUM_SCANCODES] = {0};

bool KeyPressed(int SDL_KeyCode)
{
	if (currentKeyState[SDL_KeyCode] && !previousKeyState[SDL_KeyCode])
		return (true);
	return (false);
}

bool KeyReleased(int SDL_KeyCode)
{
	if (!currentKeyState[SDL_KeyCode] && previousKeyState[SDL_KeyCode])
		return (true);
	return (false);
}

bool KeyHeld(int SDL_KeyCode)
{
	if (currentKeyState[SDL_KeyCode])
		return (true);
	return (false);
}

void UpdateKeyInputs()
{
	memcpy(previousKeyState, currentKeyState, sizeof(currentKeyState));
	const Uint8* state = SDL_GetKeyboardState(NULL);
	memcpy(currentKeyState, state, SDL_NUM_SCANCODES);
}
