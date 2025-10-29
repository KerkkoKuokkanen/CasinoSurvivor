
#include "sprite.h"

GLSprite::GLSprite(glm::vec2 pos, glm::vec2 dim, GLuint sprite, Shader *shader, int useType)
{
	Vertex vert1 = {glm::vec3(pos.x, pos.y, 0.0f),
					glm::vec2(0.0f, 1.0f)};

	Vertex vert2 = {glm::vec3(pos.x, pos.y + dim.y, 0.0f),
					glm::vec2(0.0f, 0.0f)};

	Vertex vert3 = {glm::vec3(pos.x + dim.x, pos.y + dim.y, 0.0f),
					glm::vec2(1.0f, 0.0f)};

	Vertex vert4 = {glm::vec3(pos.x + dim.x, pos.y, 0.0f),
					glm::vec2(1.0f, 1.0f)};
	GLuint index[] = {
		0, 1, 2,
		2, 3, 0
	};
	std::vector<GLuint> ind(index, index + sizeof(index) / sizeof(GLuint));
	std::vector<Vertex> verts;
	verts.push_back(vert1);
	verts.push_back(vert2);
	verts.push_back(vert3);
	verts.push_back(vert4);
	t_BoundingB boundBox = {{pos.x, pos.y + dim.y}, {pos.x + dim.x, pos.y + dim.y},
							{pos.x + dim.x, pos.y}, {pos.x, pos.y}};
	shape = new GLShapeEX(verts, ind, sprite, shader, boundBox, useType);
}

void GLSprite::Draw()
{
	shape->Draw();
}

void GLSprite::Delete()
{
	shape->Delete();
}