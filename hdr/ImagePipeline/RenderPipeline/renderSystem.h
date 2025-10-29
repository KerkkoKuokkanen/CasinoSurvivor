
#ifndef RENDER_SYSTEM_H
# define RENDER_SYSTEM_H

# include <vector>
# include <tuple>
# include <unordered_map>
# include "FBO.h"
# include "FBORender.h"
# include "box.h"

# define PI 3.1415926

namespace n_TransformTypes
{
	enum {
		NO_TRANSFORM,
		TRANSFORM_STATIC,
		TRANSFORM_CAMERA
	};
};

namespace n_SortTypes
{
	enum {
		NO_SORT,
		Y_SORT,
		DEPTH_SORT,
		DEPTH_Y_SORT,
		MULTI_LAYER,
		STRUCTURE_LAYER,
		TEXT_LAYER
	};
};

class RenderObj
{
	private:
		uint64_t uniqueKey = 0;
		friend class SystemObj;
	protected:
		void *self = NULL;
		int layer = 0;
		void AddToRenderSystem(int layer);
		void ChangeLayer(int layer);
	public:
		uint8_t renderType = 0;
		uint32_t id = 0;
		virtual void SetDrawY() {};
		virtual void BeforeDraw() {};
		virtual bool OffscreenDetection() {return (false);};
		virtual void Draw() {};
		virtual void *GetImageComponent() {return NULL;};
		virtual ~RenderObj();
		float drawY = 0.0f;
		float drawX = 0.0f;
		float drawDepth = 0.0f;
		bool drawActive = true;
};

typedef struct s_RenderLayer
{
	int						layerNumber;
	int						sortType;
	FBO						*fbo;
	FBORender				*fboRenderObj;
	std::unordered_map<uint32_t, RenderObj*> imagess;
}				t_RenderLayer;

typedef struct s_ImgDrawObj
{
	GLuint		text;
	t_BoundingB	box;
	t_BoundingB tBox;
	float		dy;
	float		dx;
	float		depth;
	t_Box		color;
}				t_ImgDrawObj;

class RenderSystem
{
	private:
		bool deleting = false;
		std::vector<t_RenderLayer> renderLayers = {};
		void AddLayerOwn(int layerNumber, int sortType);
		void SaveLayers();
		void DrawImages(int i);
		void DrawOtherObjects(std::vector<RenderObj*> &objs);
		void DrawOtherObjectsFirst(int i);
		void TransformSprites(int i, std::vector<t_ImgDrawObj> &objs);
	public:
		std::vector<std::tuple<int, int>> GetLayerData();
		void Init();
		bool AddRenderObject(RenderObj *obj, int layer, uint64_t key, uint8_t renderType);
		bool RemoveRenderObject(int layer, uint64_t key);
		void AddLayer(int layerNumber, int sortType);
		void RenderAll();
		void ClearRenderSystem();
		bool RemoveLayer(int layerNumber);
};

extern RenderSystem universalRenderingSystem;

void InitRenderSystem2();
void InitRenderSystem();

#endif
