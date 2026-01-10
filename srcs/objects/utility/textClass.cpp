
#include "textClass.h"
#include "letterTable.h"

#define USED_FONT "font3"

Text::Text(std::string text, float size, t_Point pos, t_Box color, int layer, float depth)
{
	float posY = pos.y;
	float posX = pos.x;
	float wScale = 1.0f / GetFontMaxWidth(USED_FONT);
	writing = text;
	for (int i = 0; i < text.length(); i++)
	{
		if (text[i] == '\n')
		{
			posX = pos.x;
			posY = posY - size * 0.82f;
			continue ;
		}
		if (text[i] == ' ')
		{
			posX += size * 0.4f;
			continue ;
		}
		std::tuple<t_Box, t_Point> data = LetterTable(text[i], USED_FONT);
		t_Box srcs = std::get<0>(data);
		t_Point sizes = std::get<1>(data);
		Image *img = new Image(USED_FONT, {posX, posY, size * wScale * sizes.x, size}, 0.0f, layer);
		img->position.x += img->dimentions.x * 0.5f;
		posX = img->position.x;
		img->drawDepth = depth;
		img->SetColor(color.x, color.y, color.w, color.h);
		img->SetTextureData(srcs.x, srcs.y, srcs.w - srcs.x, srcs.h - srcs.y, 0.0f);
		if (i < text.length())
		{
			std::tuple<t_Box, t_Point> data1 = LetterTable(text[i + 1], USED_FONT);
			t_Point sizes1 = std::get<1>(data1);
			if (text[i] == 'i' || text[i] == 'I')
				posX += (img->dimentions.x * 0.75f) + size * 0.05f;
			else
				posX += (img->dimentions.x * 0.5f) + size * 0.05f;
		}
		Text::text.push_back({img, text[i]});
	}
}

Text::~Text()
{
	for (int i = 0; i < text.size(); i++)
		delete text[i].img;
}
