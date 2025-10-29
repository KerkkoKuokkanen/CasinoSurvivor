
#include "ShaderClass.h"
#include "sprite.h"
#include "shape.h"
#include "Textures.h"
#include "lineDrawing.h"
#include "mouse.h"
#include "screen.h"
#include "mainTools.h"
#include "commonTools.h"
#include "image.h"
#include "structure.h"
#include "pillarBoxes.h"
#include "imageTransforms.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include "IBO.h"
#include "saveInterface.h"
#include "snapShotCreator.h"
#include "roomLoading.h"
#include "audio.h"
#include "screen.h"
#include <thread>
#include "deltaTime.h"

//2560 × 1600
#define WIDTH 1280
#define HEIGHT 720
#define FULL_SCREEN 0
#define FRAME_RATE 60

int __currentScreenWidth = 0;
int __currentScreenHeight = 0;
bool __forceAspectRatio = true;

static void SetDefaultFontSizes(float size, ImGuiIO &io)
{
	int h = 3;
	int v = 1;
	if (size > 24.01f)
	{
		h = 4;
		v = 3;
	}
	else if (size > 12.01f)
	{
		h = 3;
		v = 2;
	}
	ImFontConfig smallFontConfig;
	smallFontConfig.SizePixels = size;
	smallFontConfig.OversampleH = h;
	smallFontConfig.OversampleV = v;
	ImFont* font = io.Fonts->AddFontDefault(&smallFontConfig);
}

static void SetImguiDefaultFonts()
{
	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO();
	io.FontGlobalScale = 1.0f;

	ImFont* defaultFont = io.Fonts->AddFontDefault();

	SetDefaultFontSizes(10.0f, io);
	SetDefaultFontSizes(12.0f, io);
	SetDefaultFontSizes(14.0f, io);
	SetDefaultFontSizes(16.0f, io);
	SetDefaultFontSizes(18.0f, io);
	SetDefaultFontSizes(20.0f, io);
	SetDefaultFontSizes(22.0f, io);
	SetDefaultFontSizes(24.0f, io);
	SetDefaultFontSizes(26.0f, io);
	SetDefaultFontSizes(28.0f, io);
	SetDefaultFontSizes(30.0f, io);
	SetDefaultFontSizes(32.0f, io);
}

SDL_Window *Init()
{
	srand((unsigned int)clock() + time(0));
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
						SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
	SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, 1);

	int w = WIDTH, h = HEIGHT;
	SDL_Window *window = SDL_CreateWindow("2E", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
												w, h, SDL_WINDOW_OPENGL);
	SDL_GLContext gl_context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, gl_context);
	//SDL_GL_SetSwapInterval(1); vsync
	glDisable(GL_DEPTH_TEST);

	//ALPHA
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	SetFrameRate(FRAME_RATE);
	SetFrameTime(rounding(1000.0f / (float)FRAME_RATE));
	SDL_GetWindowSize(window, &__currentScreenWidth, &__currentScreenHeight);
	SDL_SetWindowFullscreen(window, FULL_SCREEN);
	glViewport(0, 0, __currentScreenWidth, __currentScreenHeight);
	glClearColor(0.0, 0.0, 0.0, 0.0f);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
	ImGui_ImplOpenGL3_Init("#version 330");

	SetImguiDefaultFonts();

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);

	return (window);
}

void Threads()
{
	static std::thread saveThread([]() {
		while (true) {
			SaveThread();
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	});
	static std::thread audioThread([]() {
		while (true) {
			AudioThread();
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	});
	static std::thread snapThread([]() {
		while (true) {
			UpdateSnapCreator();
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	});
}

void InitSetup(Shader *shader)
{
	InitDeltaTime(FRAME_RATE);
	Threads();
	InitAudio();
	InitShapes(shader);
	InitLines();
	InitStructure(shader);
	LoadTextures();
	SetPillarBoxes();
	ResetImageTransformations();
	InitRenderSystem();
	InitIBO();
	InitLoadRooms();
}
