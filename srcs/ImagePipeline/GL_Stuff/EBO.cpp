
#include "EBO.h"

void EBO::Create(std::vector<GLuint>& indecies)
{
	glGenBuffers(1, &ID);	//Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indecies.size() * sizeof(GLuint), indecies.data(), GL_STATIC_DRAW);
}

void EBO::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //unbinding works in a way where we bind 0 to be binded. 0 has nothing
}

void EBO::Delete()
{
	glDeleteBuffers(1, &ID);
}
