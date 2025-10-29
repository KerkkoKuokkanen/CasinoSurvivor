
#include "init.h"
#include "poller.h"
#include "keyboard.h"
#include "mouse.h"
#include "mainTools.h"
#include "envHandler.h"
#include "objBar.h"
#include "pillarBoxes.h"

SDL_Window *window = NULL;

void Utility()
{
	Poller();
	UpdateKeyInputs();
	UpdateMouse();
}

void MainLoop()
{
	/* SystemObj *obj1 = new SystemObj();
	Image *img = new Image("saving", {0.0f, 0.0f, 3.0f, 5.0f}, 0.0f, 0);
	img->SetPosition(3.0f, 3.0f);
	obj1->AddComponent(img, n_ComponentTypes::IMAGE_CLASS); */
	/* SystemObj *obj = new SystemObj();
	obj->AddComponent("ObjBar");
	obj->SetSaveable(false, 0); */
	/* SystemObj *obj = new SystemObj();
	SystemObj *obj2 = new SystemObj();
	Hitbox *add = new Hitbox();
	add->SetUpHitbox({-5.0f, 6.0f}, {2.0f, 2.0f}, 0.0f);
	obj->AddComponent(add, "Hitbox");
	RigidBody *body = new RigidBody();
	obj->AddComponent(body, "RigidBody");
	Hitbox *add2 = new Hitbox();
	add2->SetUpHitbox({5.0f, 6.0f}, {3.0f, 2.0f}, 0.0f);
	obj->AddComponent(add2, "Hitbox");
	Hitbox *add3 = new Hitbox();
	add3->SetUpHitbox({0.0f, -8.0f}, {15.0f, 2.0f}, 0.0f);
	obj2->AddComponent(add3, "Hitbox"); */
	clock_t start, end;
	while(true)
	{
		//important
		start = clock();
		ClearWindow();
		Utility();

		//goof zone
		/* if (KeyPressed(SDL_SCANCODE_1))
		{
			std::vector<Hitbox*> ret = add->Collision();
			printf("%d\n", (int)ret.size());
		} */
		//printf("%f, %f\n", GetMouseXY().x, GetMouseXY().y);

		//important
		UpdateSysEnv();
		WindowSwap(window);
		end = clock();
		SDL_Delay(figure_the_delay(start, end));
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
