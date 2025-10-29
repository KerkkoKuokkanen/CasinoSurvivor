
#include "LoadTexture.h"
#include "commonTools.h"

size_t HashSurface(SDL_Surface* surface)
{
	void* pixelData = surface->pixels;
	size_t dataSize = surface->pitch * surface->h;  // Total bytes

	return HashData64(pixelData, dataSize);
}

t_Texture LoadTexture(const char* file)
{
	t_Texture ret = {0, 0, NULL};
	SDL_Surface* surface = IMG_Load(file);
	if (!surface)
	{
		printf("IMG_Load Error: %s\n", SDL_GetError());
		return (ret);
	}
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Check the format of the surface
	GLenum format;
	if (surface->format->BytesPerPixel == 4) { // 32-bit
		if (surface->format->Rmask == 0x000000ff)
			format = GL_RGBA; // This is the correct format if the R mask is in the least significant byte
		else
			format = GL_BGRA; // If R mask is in the most significant byte, use BGRA
	} else if (surface->format->BytesPerPixel == 3) { // 24-bit
		if (surface->format->Rmask == 0x000000ff)
			format = GL_RGB;
		else
			format = GL_BGR;
	} else {
		// Unsupported format
		printf("Unsupported image format\n");
		SDL_FreeSurface(surface);
		return (ret);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, format, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	ret.text = textureID;
	ret.sur = surface;
	ret.hash = HashSurface(surface);

	return (ret);
}
