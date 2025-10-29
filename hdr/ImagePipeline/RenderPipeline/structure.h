
#ifndef STRUCTURE_H
# define STRUCTURE_H

# include "shapeEX.h"
# include "renderSystem.h"

class Structure : public RenderObj
{
	private:
		GLShape *shape = NULL;
		uint64_t type = 0;
		float GetLowY();
		bool textModdingEnabled = false;
		float angle = 0.0f;
		float width = 1.0f;
		float height = 1.0f;
		t_Point position = {0.0f, 0.0f};
		uint64_t textIndes = 0;
		GLuint texture = 0;
		uint64_t shapeData;
		int transformType = n_TransformTypes::TRANSFORM_CAMERA;
	public:
		bool detatched = false;
		Structure(uint64_t shape, uint64_t texture, int layer, bool textModding);
		Structure(uint64_t shape, std::string texture, int layer, bool textModding);
		~Structure();
		void SetDrawY() override {drawY = GetLowY();};
		bool OffscreenDetection() override;
		size_t GetSaveDataSize();
		void *CollectSaveData(void *buffer, size_t buffSize, size_t &size);
		void SetPosition(float x, float y);
		void *GetAccessToGLShape() {return ((void*)shape);};
		bool IsModdingEnabled() {return (textModdingEnabled);};
		void SetTextureData(float x, float y, float width, float height, float angle);
		void SetTransformType(int tType) {transformType = tType;};
		void SetWidth(float w);
		void SetHeight(float h);
		void SetAngle(float angle) {shape->SetRotation(angle); Structure::angle = angle;};
		t_Point GetPosition() {return (position);};
		float GetAngle() {return (angle);};
		float GetWidht() {return (width);};
		float GetHeight() {return (height);};
		uint64_t GetTexure() {return (textIndes);};
		uint64_t GetType() {return(type);};
		void Draw() override;
};

void InitStructure(Shader *usedShader);

#endif
