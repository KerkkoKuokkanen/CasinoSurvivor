
#include <SDL2/SDL.h>
#include "mouse.h"
#include "backends/imgui_impl_sdl2.h"

int scrollWheelValue = 0;

void Poller()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
		if (event.type == SDL_QUIT)
			exit(0);
		if (event.type == SDL_MOUSEWHEEL)
			scrollWheelValue = event.wheel.y;
		else
			scrollWheelValue = 0;
	}
}

int GetScrollWheelValue()
{
	return (scrollWheelValue);
}
