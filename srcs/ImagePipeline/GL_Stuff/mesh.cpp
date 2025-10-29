
#include "mesh.h"

void Mesh::CreateMesh(std::vector<Vertex>& vertecies, std::vector<GLuint>& indecies, GLuint texture, GLenum usage)
{
	Mesh::indecies = indecies;
	Mesh::texture = texture;

	VAO.Bind();

	VBO.Create(vertecies, usage);
	EBO.Create(indecies);

	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	VAO.LinkAttrib(VBO, 1, 2, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}

void Mesh::Delete()
{
	VAO.Delete();
	VBO.Delete();
	EBO.Delete();
}