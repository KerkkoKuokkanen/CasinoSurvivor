
#include "init.h"
#include "poller.h"
#include "keyboard.h"
#include "mouse.h"
#include "mainTools.h"
#include "envHandler.h"
#include "objBar.h"
#include "deltaTime.h"
#include "image.h"

SDL_Window *window = NULL;

void Utility()
{
	Poller();
	UpdateKeyInputs();
	UpdateMouse();
}

void MainLoop()
{
	/* SystemObj *obj = new SystemObj();
	obj->AddComponent("ObjBar");
	obj->SetSaveable(true, 0); */
	const Uint64 freq = SDL_GetPerformanceFrequency();
	Uint64 start = SDL_GetPerformanceCounter(), end = SDL_GetPerformanceCounter();
	while(true)
	{
		//DeltaTime
		start = SDL_GetPerformanceCounter();
		Uint64 deltaTicks = start - end;
		SetDeltaTime((float)((double)deltaTicks / (double)freq));
		end = SDL_GetPerformanceCounter();

		//important
		ClearWindow();
		Utility();

		//important
		UpdateSysEnv();
		WindowSwap(window);
		SDL_Delay(figure_the_delay(start, freq));
	}
}

int main()
{
	window = Init();
	Shader shader("shaders/sprite_vert.glsl", "shaders/sprite_frag.glsl");
	InitSetup(&shader);
	universalRenderingSystem.Init();
	LoadEngineRoom();
	MainLoop();
	return (0);
}
