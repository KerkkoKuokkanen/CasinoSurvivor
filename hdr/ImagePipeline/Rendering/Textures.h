
#ifndef TEXTURES_H
# define TEXTURES_H

# include <OpenGL/gl3.h>
# include "../frameworks/SDL2/SDL.h"
# include <iostream>
# include <unordered_map>

typedef struct s_Texture
{
	uint64_t hash;
	GLuint text;
	SDL_Surface *sur;
}				t_Texture;

typedef struct s_TestTexts
{
	t_Texture tile;
	t_Texture hamis;
	t_Texture everyColor;
	t_Texture colorTester;
	t_Texture testAtlas;
	t_Texture selectedObj;
}				t_TestTexts;

extern t_TestTexts gameTestTextures;

void LoadTextures();
GLuint GetTextureGLSign(uint64_t hash);
GLuint GetTextureGLSign(std::string name);
t_Texture GetTextureGLData(uint64_t hash);
t_Texture GetTextureGLData(std::string name);
std::unordered_map<std::string, t_Texture> GetAllTextures();

#endif
