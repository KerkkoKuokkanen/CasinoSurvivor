
#include "renderSystem.h"
#include <OpenGL/gl3.h>
#include "Textures.h"
#include "camera.h"
#include "image.h"
#include "vectorTools.h"
#include "imageTransforms.h"
#include <algorithm>
#include "screen.h"

Shader *defaultImageShader = NULL;

GLint tex0 = 0;
GLint tex1 = 0;
GLint tex2 = 0;
GLint tex3 = 0;

GLuint dummy = 0;
GLuint vao, vbo, ebo;

struct SpriteVertex {
	glm::vec2 position;
	glm::vec2 texCoord;
	glm::vec4 color;
	float texIndex;
};

void InitSpriteDrawing()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (void*)offsetof(SpriteVertex, position));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (void*)offsetof(SpriteVertex, texCoord));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (void*)offsetof(SpriteVertex, color));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (void*)offsetof(SpriteVertex, texIndex));

	glBindVertexArray(0);
}

static float ImgGetLowY(t_BoundingB &data)
{
	float d_drawY = data.leftBottom.y;
	if (d_drawY > data.leftTop.y)
		d_drawY = data.leftTop.y;
	if (d_drawY > data.rightBottom.y)
		d_drawY = data.rightBottom.y;
	if (d_drawY > data.rightTop.y)
		d_drawY = data.rightTop.y;
	return (d_drawY);
}

static float ImgGetHighY(t_BoundingB &data)
{
	float d_drawY = data.leftBottom.y;
	if (d_drawY < data.leftTop.y)
		d_drawY = data.leftTop.y;
	if (d_drawY < data.rightBottom.y)
		d_drawY = data.rightBottom.y;
	if (d_drawY < data.rightTop.y)
		d_drawY = data.rightTop.y;
	return (d_drawY);
}

static float ImgGetLowX(t_BoundingB &data)
{
	float d_drawx = data.leftBottom.x;
	if (d_drawx > data.leftTop.x)
		d_drawx = data.leftTop.x;
	if (d_drawx > data.rightBottom.x)
		d_drawx = data.rightBottom.x;
	if (d_drawx > data.rightTop.x)
		d_drawx = data.rightTop.x;
	return (d_drawx);
}

static float ImgGetHighX(t_BoundingB &data)
{
	float d_drawx = data.leftBottom.x;
	if (d_drawx < data.leftTop.x)
		d_drawx = data.leftTop.x;
	if (d_drawx < data.rightBottom.x)
		d_drawx = data.rightBottom.x;
	if (d_drawx < data.rightTop.x)
		d_drawx = data.rightTop.x;
	return (d_drawx);
}

static bool SortLayerXSort(t_ImgDrawObj &obj1, t_ImgDrawObj &obj2)
{
	return (obj1.dx > obj2.dx);
}

static bool SortLayerYSort(t_ImgDrawObj &obj1, t_ImgDrawObj &obj2)
{
	if (std::fabs(obj1.dy - obj2.dy) < 0.0001f)
		return (SortLayerXSort(obj1, obj2));
	return (obj1.dy > obj2.dy);
}

static bool SortLayerDepthSort(t_ImgDrawObj &obj1, t_ImgDrawObj &obj2)
{
	return (obj1.depth < obj2.depth);
}

static bool SortLayerDepthAndYSort(t_ImgDrawObj &obj1, t_ImgDrawObj &obj2)
{
	if ((std::fabs(obj1.depth - obj2.depth) < 0.0001f))
		return SortLayerYSort(obj1, obj2);
	return (obj1.depth < obj2.depth);
}

static void TransformTextureData(t_BoundingB &texData, textData data)
{
	texData = {{data.x, data.y + data.h}, {data.x + data.w, data.y + data.h}, {data.x + data.w, data.y}, {data.x, data.y}};
	texData.leftTop = VectorRotate(texData.leftTop, data.a);
	texData.rightTop = VectorRotate(texData.rightTop, data.a);
	texData.rightBottom = VectorRotate(texData.rightBottom, data.a);
	texData.leftBottom = VectorRotate(texData.leftBottom, data.a);
}

static bool ImageOnScreen(t_BoundingB def)
{
	float hx = ImgGetHighX(def);
	float lx = ImgGetLowX(def);
	float hy = ImgGetHighY(def);
	float ly = ImgGetLowY(def);
	if (hx < -1.0f)
		return (false);
	if (lx > 1.0f)
		return (false);
	if (hy < (-1.0f * (9.0f / 16.0f)))
		return (false);
	if (ly > (1.0f * (9.0f / 16.0f)))
		return (false);
	return (true);
}

void RenderSystem::TransformSprites(int i, std::vector<t_ImgDrawObj> &objs)
{
	for (auto [key, obj] : renderLayers[i].imagess)
	{
		t_ImgDrawObj add;
		Image *img = (Image*)obj->GetImageComponent();
		if (img->drawActive == false)
			continue ;
		add.depth = img->drawDepth;
		t_Point pos = img->position;
		t_Point dim = {img->dimentions.x * 0.5f, img->dimentions.y * 0.5f};
		if (img->GetTransformType() == n_TransformTypes::TRANSFORM_CAMERA)
			pos = TransformCoordinateToScreenSpaceCamera(pos.x, pos.y);
		else
			pos = TransformCoordinateToScreenSpace(pos.x, pos.y);
		pos.y *= (9.0f / 16.0f);
		t_BoundingB def = img->getBoundingBox();
		def.leftTop = VectorRotate(def.leftTop, img->angle);
		def.rightTop = VectorRotate(def.rightTop, img->angle);
		def.rightBottom = VectorRotate(def.rightBottom, img->angle);
		def.leftBottom = VectorRotate(def.leftBottom, img->angle);
		def.leftTop = {def.leftTop.x + pos.x, def.leftTop.y + pos.y};
		def.rightTop = {def.rightTop.x + pos.x, def.rightTop.y + pos.y};
		def.rightBottom = {def.rightBottom.x + pos.x, def.rightBottom.y + pos.y};
		def.leftBottom = {def.leftBottom.x + pos.x, def.leftBottom.y + pos.y};
		if (!ImageOnScreen(def))
			continue ;
		add.text = img->GetGLTexture();
		add.color = img->GetColor();
		add.dx = ImgGetLowX(def);
		add.dy = ImgGetLowY(def);
		add.box = def;
		TransformTextureData(add.tBox, img->GetTextureData());
		objs.push_back(add);
	}
	if (renderLayers[i].sortType == n_SortTypes::DEPTH_SORT)
		std::sort(objs.begin(), objs.end(), SortLayerDepthSort);
	else if (renderLayers[i].sortType == n_SortTypes::Y_SORT)
		std::sort(objs.begin(), objs.end(), SortLayerYSort);
	else if (renderLayers[i].sortType == n_SortTypes::DEPTH_Y_SORT)
		std::sort(objs.begin(), objs.end(), SortLayerDepthAndYSort);
}

static void BindTextures(std::vector<GLuint> &activeTextures)
{
	for (size_t ind = 0; ind < 4; ind++)
	{
		if (ind >= activeTextures.size())
		{
			glActiveTexture(GL_TEXTURE0 + ind);
			glBindTexture(GL_TEXTURE_2D, dummy);
		}
		else
		{
			glActiveTexture(GL_TEXTURE0 + ind);
			glBindTexture(GL_TEXTURE_2D, activeTextures[ind]);
		}
	}
}

static float CheckTextureBinding(std::vector<GLuint> &activeTextures, GLuint texture)
{
	for (int i = 0; i < activeTextures.size(); i++)
	{
		if (activeTextures[i] == texture)
			return (float)i;
	}
	if (activeTextures.size() < 3)
	{
		activeTextures.push_back(texture);
		return (float)(activeTextures.size() - 1);
	}
	return (-1.0f);
}

void RenderSystem::DrawImages(int i)
{
	if (renderLayers[i].imagess.size() == 0)
		return ;

	defaultImageShader->Activate();

	std::vector<SpriteVertex> vertices;
	std::vector<GLuint> indices;
	std::vector<t_ImgDrawObj> objs = {};
	std::vector<GLuint> activeTextures = {};
	GLuint vertexOffset = 0;
	TransformSprites(i, objs);

	auto flushBatch = [&]()
	{
		BindTextures(activeTextures);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(SpriteVertex), vertices.data(), GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_DYNAMIC_DRAW);

		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		vertices.clear();
		indices.clear();
		vertexOffset = 0;
		activeTextures.clear();
	};

	for (t_ImgDrawObj &item : objs)
	{
		float textIndex = CheckTextureBinding(activeTextures, item.text);
		if (textIndex < -0.1f)
		{
			flushBatch();
			textIndex = 0.0f;
			activeTextures.push_back(item.text);
		}
		t_BoundingB bBox = item.box;
		t_BoundingB tBox = item.tBox;
		t_Box color = item.color;
		SpriteVertex v1 = {{bBox.leftTop.x, bBox.leftTop.y }, {tBox.leftTop.x, tBox.leftTop.y}, {color.x, color.y, color.w, color.h}, textIndex};
		SpriteVertex v2 = {{bBox.rightTop.x, bBox.rightTop.y}, {tBox.rightTop.x, tBox.rightTop.y}, {color.x, color.y, color.w, color.h}, textIndex};
		SpriteVertex v3 = {{bBox.rightBottom.x, bBox.rightBottom.y}, {tBox.rightBottom.x, tBox.rightBottom.y}, {color.x, color.y, color.w, color.h}, textIndex};
		SpriteVertex v4 = {{bBox.leftBottom.x, bBox.leftBottom.y}, {tBox.leftBottom.x, tBox.leftBottom.y}, {color.x, color.y, color.w, color.h}, textIndex};
		vertices.push_back(v1);
		vertices.push_back(v2);
		vertices.push_back(v3);
		vertices.push_back(v4);
		indices.push_back(vertexOffset + 0);
		indices.push_back(vertexOffset + 1);
		indices.push_back(vertexOffset + 2);
		indices.push_back(vertexOffset + 2);
		indices.push_back(vertexOffset + 3);
		indices.push_back(vertexOffset + 0);
		vertexOffset += 4;
	}
	if (vertices.size() > 0)
		flushBatch();
}

void InitRenderSystem2()
{
	defaultImageShader = new Shader("shaders/image_vert.glsl", "shaders/image_frag.glsl");
	defaultImageShader->Activate();
	InitSpriteDrawing();
	tex0 = glGetUniformLocation(defaultImageShader->ID, "uTextures[0]");
	tex1 = glGetUniformLocation(defaultImageShader->ID, "uTextures[1]");
	tex2 = glGetUniformLocation(defaultImageShader->ID, "uTextures[2]");
	tex3 = glGetUniformLocation(defaultImageShader->ID, "uTextures[3]");
	glUniform1i(tex0, 0);
	glUniform1i(tex1, 1);
	glUniform1i(tex2, 2);
	glUniform1i(tex3, 3); 
	dummy = GetTextureGLSign("everyColor");
}
