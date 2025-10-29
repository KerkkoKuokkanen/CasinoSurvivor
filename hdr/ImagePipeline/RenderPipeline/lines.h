
#ifndef LINES_CLASS_H
# define LINES_CLASS_H

# include "box.h"
# include "renderSystem.h"

class Lines : public RenderObj
{
	private:
		glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
		std::vector<t_Point> lines;
	public:
		bool active = true;
		Lines(int layer = 0);
		void SetColor(float r, float g, float b, float a);
		void SetLines(std::vector<t_Point> &lines);
		void Draw() override;
};

#endif
