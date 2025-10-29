
#include "VAO.h"

VAO::VAO()
{
	glGenVertexArrays(1, &ID);	//VAO is a thing we use to tell opengl what VBO we use
}

void VAO::LinkAttrib(VBO &VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	VBO.Bind();
	// we gave opengl a VAO earlier and now this specifies how to use the VAO
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	//we enable the attribute and give it the postition to use
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

void VAO::Bind()
{
	glBindVertexArray(ID);
}

void VAO::Unbind()
{
	glBindVertexArray(0);
}

void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}
