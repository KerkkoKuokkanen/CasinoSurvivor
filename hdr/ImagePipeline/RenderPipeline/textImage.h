
#ifndef TEXT_IMAGE_H
# define TEXT_IMAGE_H

# include "renderSystem.h"
# include "box.h"

class TextImage : public RenderObj
{
	private:
		t_Point position;
		std::string text;
		int r = 255, g = 255, b = 255, a = 255;
		int fontSize = 0;
	public:
		TextImage(std::string text, t_Point pos, int layer);
		void ChangeText(std::string text);
		void SetRBGA(int r, int g, int b, int a);
		void SetFontSize(int size);
		void Draw() override;
};

#endif
