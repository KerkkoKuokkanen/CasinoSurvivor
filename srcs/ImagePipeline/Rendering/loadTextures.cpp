
#include "LoadTexture.h"
#include "Textures.h"
#include <iostream>
#include <filesystem>
#include <string>
#include "unordered_map"

namespace fs = std::filesystem;

std::unordered_map<uint64_t, t_Texture> textures1;
std::unordered_map<std::string, t_Texture> textures2;

void loadSpritesFromFolder(const std::string& folderPath)
{
	try
	{
		for (const auto& entry : fs::recursive_directory_iterator(folderPath))
		{
			if (fs::is_regular_file(entry.path()))
			{
				std::string filepath = entry.path().string();
				std::string spriteName = entry.path().stem().string(); 
				if (entry.path().extension() == ".png")
				{
					t_Texture ret = LoadTexture(filepath.c_str());
					textures1[ret.hash] = ret;
					textures2[spriteName] = ret;
				}
			}
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error accessing directory: " << e.what() << std::endl;
	}
}

std::unordered_map<std::string, t_Texture> GetAllTextures()
{
	return (textures2);
}

t_Texture GetTextureGLData(uint64_t hash)
{
	auto it = textures1.find(hash);
	if (it == textures1.end())
		return ((t_Texture){0, 0, NULL});
	return (textures1[hash]);
}

t_Texture GetTextureGLData(std::string name)
{
	auto it = textures2.find(name);
	if (it == textures2.end())
		return ((t_Texture){0, 0, NULL});
	return (textures2[name]);
}

GLuint GetTextureGLSign(uint64_t hash)
{
	return (textures1[hash].text);
}

GLuint GetTextureGLSign(std::string name)
{
	return (textures2[name].text);
}

void LoadTextures()
{
	loadSpritesFromFolder("assets/sprites");
}
