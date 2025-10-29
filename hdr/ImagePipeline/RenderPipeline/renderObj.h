
#ifndef RENDER_OBJ_H
# define RENDER_OBJ_H

# include "systemObj.h"

class RenderObj
{
	private:
		uint32_t uniqueKey = 0;
	protected:
		int layer = 0;
		void AddToRenderSystem(int layer);
	public:
		SystemObj *self = NULL;
		uint32_t id = 0;
		virtual void SetDrawY() = 0;
		virtual bool OffscreenDetection() = 0;
		virtual void Draw() = 0;
		virtual ~RenderObj();
		float drawY = 0.0f;
		float drawDepth = 0.0f;
		bool drawActive = true;
};

#endif
