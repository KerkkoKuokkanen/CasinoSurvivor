
#include "VBO.h"

void VBO::Create(std::vector<Vertex>& vertecies, GLenum usage)
{
	glGenBuffers(1, &ID);	//buffers are a way to send data to the GPU
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	//allocates memory to the GPU and then copies the data to that buffer from the third parameter
	//the fourth argument is a hint to the opengl of the usecase of our buffer.
	//this hint gives the opengl information on how to achieve good performance so you should give a correct type of hint
	glBufferData(GL_ARRAY_BUFFER, vertecies.size() * sizeof(Vertex), vertecies.data(), usage);
}

void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbinding works in a way where we bind 0 to be binded. 0 has nothing
}

void VBO::Delete()
{
	glDeleteBuffers(1, &ID);
}
