
#include "image.h"
#include "Textures.h"
#include "imageTransforms.h"

Image::Image(std::string texture, t_Box rect, float angle, int layer)
{
	t_Texture data = GetTextureGLData(texture);
	Image::texture = data.text;
	dimentions = {rect.w, rect.h};
	textIndex = data.hash;
	Image::angle = angle;
	position = {rect.x, rect.y};
	AddToRenderSystem(layer);
}

Image::Image(uint64_t texture, t_Box rect, float angle, int layer)
{
	Image::texture = GetTextureGLSign(texture);
	dimentions = {rect.w, rect.h};
	textIndex = texture;
	Image::angle = angle;
	position = {rect.x, rect.y};
	AddToRenderSystem(layer);
}

void Image::SetColor(float r, float g, float b, float a)
{
	color.x = r;
	color.y = g;
	color.w = b;
	color.h = a;
}

void Image::SetTexture(std::string name)
{
	t_Texture res = GetTextureGLData(name);
	textIndex = res.hash;
	texture = res.text;
}

void Image::SetTexture(uint64_t hash)
{
	textIndex = hash;
	texture = GetTextureGLSign(hash);
}

void Image::SetTextureData(float x, float y, float w, float h, float a)
{
	textureData = {x, y, w, h, a};
}

textData Image::GetTextureData()
{
	return (textureData);
}

size_t Image::GetSaveDataSize()
{
	size_t dataSize = sizeof(float) * 16 + sizeof(uint64_t) + sizeof(int) * 2;
	return (dataSize);
}

void *Image::CollectSaveData(void *buffer, size_t buffSize, size_t &size)
{
	size_t dataSize = sizeof(float) * 16 + sizeof(uint64_t) + sizeof(int) * 2;
	size = dataSize;
	if (dataSize > buffSize)
		return (NULL);
	uint8_t *byteData = (uint8_t *)buffer;
	size_t offset = 0;
	memcpy(byteData + offset, &position.x, sizeof(float)); offset += sizeof(float);
	memcpy(byteData + offset, &position.y, sizeof(float)); offset += sizeof(float);
	memcpy(byteData + offset, &dimentions.x, sizeof(float)); offset += sizeof(float);
	memcpy(byteData + offset, &dimentions.y, sizeof(float)); offset += sizeof(float);
	memcpy(byteData + offset, &angle, sizeof(float)); offset += sizeof(float);
	memcpy(byteData + offset, &color.x, sizeof(float)); offset += sizeof(float);
	memcpy(byteData + offset, &color.y, sizeof(float)); offset += sizeof(float);
	memcpy(byteData + offset, &color.w, sizeof(float)); offset += sizeof(float);
	memcpy(byteData + offset, &color.h, sizeof(float)); offset += sizeof(float);
	memcpy(byteData + offset, &textIndex, sizeof(uint64_t)); offset += sizeof(uint64_t);
	memcpy(byteData + offset, &transformType, sizeof(int)); offset += sizeof(int);
	memcpy(byteData + offset, &layer, sizeof(int)); offset += sizeof(int);
	memcpy(byteData + offset, &textureData.x, sizeof(float)); offset += sizeof(float);
	memcpy(byteData + offset, &textureData.y, sizeof(float)); offset += sizeof(float);
	memcpy(byteData + offset, &textureData.w, sizeof(float)); offset += sizeof(float);
	memcpy(byteData + offset, &textureData.h, sizeof(float)); offset += sizeof(float);
	memcpy(byteData + offset, &textureData.a, sizeof(float)); offset += sizeof(float);
	return (buffer);
}

void *Image::GetImageComponent()
{
	return this;
}

t_BoundingB Image::getBoundingBox()
{
	if (bBoxSet)
		return (bBox);
	t_Point dim = {dimentions.x * 0.5f, dimentions.y * 0.5f};
	if (transformType == n_TransformTypes::TRANSFORM_CAMERA)
	{
		dim.x = TransformWidthToCameraSpace(dim.x) * 0.1f;
		dim.y = TransformHeightToCameraSpace(dim.y) * 0.1f;
	}
	else
	{
		dim.x *= 0.1f;
		dim.y *= 0.1f;
	}
	t_BoundingB def = {{-dim.x, -dim.y}, {dim.x, -dim.y},
						{dim.x, dim.y}, {-dim.x, dim.y}};
	return (def);
}

void Image::SetBoundingBox(t_BoundingB bbox)
{
	bBoxSet = true;
	bBox = bbox;
}
