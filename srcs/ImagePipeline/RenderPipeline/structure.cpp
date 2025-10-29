
#include "structure.h"
#include "imageTransforms.h"
#include "convexOverlap.h"
#include "Textures.h"

Shader *defaultStructureShader = NULL;

float Structure::GetLowY()
{
	t_BoundingB data = shape->GetRotatedBoundingBox();
	float d_drawY = data.leftBottom.y;
	if (d_drawY > data.leftTop.y)
		d_drawY = data.leftTop.y;
	if (d_drawY > data.rightBottom.y)
		d_drawY = data.rightBottom.y;
	if (d_drawY > data.rightTop.y)
		d_drawY = data.rightTop.y;
	return (d_drawY);
}

void Structure::SetWidth(float w)
{
	shape->SetWidth(w);
	width = w;
}

void Structure::SetHeight(float h)
{
	shape->SetHeight(h);
	height = h;
}

Structure::Structure(uint64_t sshape, std::string texture, int layer, bool textModding)
{
	renderType = 1;
	type = sshape;
	t_DataForShape &data = GetShapeDataWithKey(sshape);
	t_Texture date = GetTextureGLData(texture);
	Structure::texture = date.text;
	if (!textModding)
		shape = new GLShape(data.vertexData, data.indexData, Structure::texture, defaultStructureShader, data.bBox, 0);
	else
		shape = new GLShapeEX(data.vertexData, data.indexData, Structure::texture, defaultStructureShader, data.bBox, 0);
	drawY = GetLowY();
	AddToRenderSystem(layer);
	textModdingEnabled = textModding;
	position = {0.0f, 0.0f};
	Structure::textIndes = date.hash;
	shapeData = sshape;
}

Structure::Structure(uint64_t sshape, uint64_t texture, int layer, bool textModding)
{
	renderType = 1;
	type = sshape;
	t_DataForShape &data = GetShapeDataWithKey(sshape);
	Structure::texture = GetTextureGLSign(texture);
	if (!textModding)
		shape = new GLShape(data.vertexData, data.indexData, Structure::texture, defaultStructureShader, data.bBox, 0);
	else
		shape = new GLShapeEX(data.vertexData, data.indexData, Structure::texture, defaultStructureShader, data.bBox, 0);
	drawY = GetLowY();
	AddToRenderSystem(layer);
	textModdingEnabled = textModding;
	position = {0.0f, 0.0f};
	Structure::textIndes = texture;
	shapeData = sshape;
}

void Structure::SetPosition(float x, float y)
{
	if (shape == NULL)
		return ;
	t_Point used;
	switch (transformType)
	{
		case n_TransformTypes::TRANSFORM_CAMERA:
			used = TransformCoordinateToScreenSpaceCamera(x, y);
			break ;
		case n_TransformTypes::TRANSFORM_STATIC:
			used = TransformCoordinateToScreenSpace(x, y);
		default:
			used = {x, y};
			break ;
	}
	shape->SetPosition(used.x, used.y);
	position = {x, y};
}

void Structure::SetTextureData(float x, float y, float width, float height, float angle)
{
	if (textModdingEnabled == false)
		return ;
	GLShapeEX *cast = (GLShapeEX*)shape;
	cast->SetAll(x, y, width, height, angle);
}

bool Structure::OffscreenDetection()
{
	if (shape == NULL)
		return (true);
	t_BoundingB data = shape->GetRotatedBoundingBox();
	if (!ReactangleScreenOverlap(data))
		return (true);
	return (false);
}

void Structure::Draw()
{
	if (shape == NULL)
		return ;
	shape->Draw();
}

Structure::~Structure()
{
	if (shape != NULL)
		delete (shape);
}

void InitStructure(Shader *usedShader)
{
	defaultStructureShader = usedShader;
}

size_t Structure::GetSaveDataSize()
{
	size_t dataSize = sizeof(float) * 3 + sizeof(uint64_t) + sizeof(int) + sizeof(uint64_t);
	return (dataSize);
}

void *Structure::CollectSaveData(void *buffer, size_t buffSize, size_t &size)
{
	size_t dataSize = sizeof(float) * 3 + sizeof(GLuint) + sizeof(int) + sizeof(uint64_t);
	size = dataSize;
	if (dataSize > buffSize)
		return (NULL);
	uint8_t *byteData = (uint8_t *)buffer;
	size_t offset = 0;
	memcpy(byteData + offset, &position.x, sizeof(float)); offset += sizeof(float);
	memcpy(byteData + offset, &position.y, sizeof(float)); offset += sizeof(float);
	memcpy(byteData + offset, &angle, sizeof(float)); offset += sizeof(float);
	memcpy(byteData + offset, &texture, sizeof(uint64_t)); offset += sizeof(uint64_t);
	memcpy(byteData + offset, &shapeData, sizeof(uint64_t)); offset += sizeof(uint64_t);
	memcpy(byteData + offset, &layer, sizeof(int));
	return (buffer);
}
