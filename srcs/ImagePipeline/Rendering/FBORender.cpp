
#include "FBORender.h"
#include "screen.h"

void FBORender::Draw()
{
	shader->Activate();
	mesh.VAO.Bind();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh.texture);

	glUniform2f(uniform, (float)__currentScreenWidth, (float)__currentScreenHeight);

	glDrawElements(GL_TRIANGLES, mesh.indecies.size(), GL_UNSIGNED_INT, 0);
}

FBORender::FBORender(Shader *shader)
{
	FBORender::shader = shader;
	Vertex vert1 = {glm::vec3(-1.0f, 1.0f, 0.0f),
					glm::vec2(0.0f, 1.0f)};

	Vertex vert2 = {glm::vec3(-1.0f, -1.0f, 0.0f),
					glm::vec2(0.0f, 0.0f)};

	Vertex vert3 = {glm::vec3(1.0f, -1.0f, 0.0f),
					glm::vec2(1.0f, 0.0f)};

	Vertex vert4 = {glm::vec3(1.0f, 1.0f, 0.0f),
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
	mesh.CreateMesh(verts, ind, 0, GL_STATIC_DRAW);
	uniform = glGetUniformLocation(shader->ID, "resolution");
}

FBORender::~FBORender()
{
	mesh.Delete();
}
