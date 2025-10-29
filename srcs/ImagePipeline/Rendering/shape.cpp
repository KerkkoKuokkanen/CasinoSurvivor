
#include "shape.h"
#include "commonTools.h"
#include "convexOverlap.h"
#include "lineDrawing.h"

Shader *defaultShader;
static int colorLocation = 0;
static int positionLocation = 0;
static int scaleLocation = 0;
static int rotationLocation = 0;
static int pivotPointLocation = 0;

static t_Point GetCenter(t_BoundingB &boundBox)
{
	float xCenter = (boundBox.leftBottom.x + boundBox.leftTop.x + boundBox.rightBottom.x + boundBox.rightTop.x) / 4.0f;
	float yCenter = (boundBox.leftBottom.y + boundBox.leftTop.y + boundBox.rightBottom.y + boundBox.rightTop.y) / 4.0f;
	return ((t_Point){xCenter, yCenter});
}

t_Point GLShape::GetCenterBB()
{
	return (pivotPoint);
}

GLShape::GLShape(std::vector<Vertex> &verts, std::vector<GLuint> &inds, GLuint texture, Shader *shader, t_BoundingB boundingBox, int useType)
{
	GLShape::shader = (shader == NULL) ? defaultShader : shader;
	GLenum usage = GL_STATIC_DRAW;
	if (useType == n_DrawUseTypes::DATA_CHANGES_LOT)
		usage = GL_DYNAMIC_DRAW;
	else if (useType == n_DrawUseTypes::ONE_USE_ONLY)
		usage = GL_STREAM_DRAW;
	mesh.CreateMesh(verts, inds, texture, usage);
	boundBox = boundingBox;
	rotatedBBox = boundBox;
	width = 1.0f;
	height = 1.0f;
	pivotPoint = GetCenter(boundBox);
}

void GLShape::Draw()
{
	if (shader == NULL)
		return ;
	shader->Activate();
	mesh.VAO.Bind();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh.texture);

	glUniform4f(colorLocation, imageColor.x, imageColor.y, imageColor.z, imageColor.w);
	glUniform2f(positionLocation, position.x, position.y);
	glUniform2f(scaleLocation, width, height);
	glUniform1f(rotationLocation, -angle);
	glUniform2f(pivotPointLocation, pivotPoint.x, pivotPoint.y);

	glDrawElements(GL_TRIANGLES, mesh.indecies.size(), GL_UNSIGNED_INT, 0);
}

void GLShape::SetPosition(float x, float y)
{
	position = {x - pivotPoint.x, y - pivotPoint.y};
	UpdateBoundingBox();
}

void GLShape::SetRotation(float angle)
{
	GLShape::angle = angle;
	UpdateBoundingBox();
}

void GLShape::Delete()
{
	mesh.Delete();
}

void InitShapes(Shader *shaderProgram)
{
	defaultShader = shaderProgram;
	colorLocation = glGetUniformLocation(shaderProgram->ID, "imageColor");
	positionLocation = glGetUniformLocation(shaderProgram->ID, "uPosition");
	scaleLocation = glGetUniformLocation(shaderProgram->ID, "uScale");
	rotationLocation = glGetUniformLocation(shaderProgram->ID, "uRotation");
	pivotPointLocation = glGetUniformLocation(shaderProgram->ID, "uPivot");
}
