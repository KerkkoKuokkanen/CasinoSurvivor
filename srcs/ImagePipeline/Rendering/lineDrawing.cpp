
#include "ShaderClass.h"
#include <glm/gtc/type_ptr.hpp>
#include "box.h"
#include <vector>

static Shader *lineShader = NULL;
static GLint colorLocation = 0;

void InitLines()
{
	lineShader = new Shader("shaders/line_vert.glsl", "shaders/line_frag.glsl");
	colorLocation = glGetUniformLocation(lineShader->ID, "lineColor");
}

void DrawLinesWithColor(std::vector<t_Point> &points, glm::vec4 color)
{
	int size = (int)points.size();
	if (size == 0)
		return ;
	float* lineVertices = new float[size + size];
	int counter = 0;
	for (int i = 0; i < size; i++)
	{
		lineVertices[counter] = points[i].x;
		lineVertices[counter + 1] = points[i].y;
		counter += 2;
	}
	// Create a Vertex Array Object (VAO) and Vertex Buffer Object (VBO) for the line
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	// Bind the VBO and copy the line vertices to the buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (size + size), lineVertices, GL_STREAM_DRAW);

	// Specify the layout of the vertex data (location = 0 in the vertex shader)
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Use the shader program for drawing the line
	glUseProgram(lineShader->ID);

	glUniform4fv(colorLocation, 1, glm::value_ptr(color));

	// Bind the VAO and draw the line
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINE_STRIP, 0, size);  // Drawing two vertices as a line

	// Unbind and clean up
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	delete[] lineVertices;
}
