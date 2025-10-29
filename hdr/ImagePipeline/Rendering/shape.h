

#ifndef SHAPE_H
# define SHAPE_H

# include "mesh.h"
# include "ShaderClass.h"
# include <glm/gtc/matrix_transform.hpp>
# include "box.h"

namespace n_DrawUseTypes
{
	enum {
		DATA_STAYS_MOSTLY_THE_SAME,
		DATA_CHANGES_LOT,
		ONE_USE_ONLY
	};
};

typedef struct	s_DataForShape
{
	std::vector<Vertex>	vertexData;
	std::vector<GLuint>	indexData;
	t_BoundingB			bBox;
}				t_DataForShape;

class GLShape
{
	private:
		Shader *shader = NULL;
		t_BoundingB boundBox;
		t_BoundingB rotatedBBox;
		glm::vec4 imageColor = {1.0f, 1.0f, 1.0f, 1.0f};
		float angle = 0.0f;
		float width = 0.0f;
		float height = 0.0f;
		t_Point pivotPoint = {0.0f, 0.0f};
		t_Point position = {0.0f, 0.0f};
		t_Point scaleDirection = {0.0f, 1.0f};
		float scaleDir = 1.0f;
		float scalePerp = 1.0f;
		void CalculateBBoxPosition(t_BoundingB &box);
		void CalculateBBoxWidth(t_BoundingB &box);
		void CalculateBBoxHeight(t_BoundingB &box);
		void CalculateBBoxRotation(t_BoundingB &box);
		t_Point CalculateBBoxDirScalePoint(t_Point pos, t_Point center, t_Point normal, float multi);
		void CalculateBBoxDirScale(t_BoundingB &box);
		void CalculateBBoxPerpScale(t_BoundingB &box);
		void UpdateBoundingBox();
	protected:
		t_Point GetPivotPoint() {return (pivotPoint);};
	public:
		Mesh mesh;
		GLShape(std::vector<Vertex> &verts, std::vector<GLuint> &inds, GLuint texture, Shader *shader, t_BoundingB boundingBox, int useType);
		~GLShape() {Delete();};

		//setters:
		void SetPosition(float x, float y);
		void SetRotation(float angle);
		void SetHeight(float h);
		void SetWidth(float w);
		void SetColor(float r, float g, float b, float a) {imageColor = {r, g, b, a};};
		void SetDirScaleDir(t_Point dir);
		void SetDirScaleWidth(float width);
		void SetDirScaleHeight(float height);
		void SetTexture(GLuint texture) {mesh.texture = texture;};

		//getters:
		float GetScaleDir() {return (scaleDir);};
		float GetScalePerp() {return (scalePerp);};
		t_Point GetScaleDirection() {return (scaleDirection);};
		GLuint GetTexture() {return (mesh.texture);};
		t_BoundingB GetBoundingBox() {return(boundBox);};
		t_BoundingB GetRotatedBoundingBox() {return(rotatedBBox);};
		t_Point GetCenterBB();
		float GetAngle() {return(angle);};
		glm::vec4 GetColor() {return(imageColor);};
		float GetWidth() {return(width);};
		float GetHeight() {return(height);};

		//functionality:
		void Draw();
		void Delete();
};

t_DataForShape &GetShapeDataWithKey(uint64_t key);
uint64_t CreateGLShapeData(std::vector<float> &points);
void AddShapeToHolder(t_DataForShape data, uint64_t key);
void InitShapes(Shader *shaderProgram);

#endif
