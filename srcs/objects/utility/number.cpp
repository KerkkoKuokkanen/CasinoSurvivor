
#include "number.h"

void Number::SetColor(float r, float g, float b, float a)
{
	for (Image *i : images)
		i->SetColor(r, g, b, a);
}

void Number::SetPosition(t_Point pos)
{
	position = pos;
	for (int i = 0; i < images.size(); i++)
		images[i]->position = {pos.x + (size * (float)i), pos.y};
}

Number::Number(t_Point position, std::string number, float size, t_Box color, float depth, bool staticImage)
{
	Number::size = size;
	Number::position = position;
	float add = 0.1f;
	num = std::stoi(number);
	for (int i = 0; i < number.length(); i++)
	{
		char n = number[i];
		int l = n - '0';
		t_Box rect = {add * (float)l, 0.0f, 0.1f, 1.0f};
		Image *img = new Image("numbers", {position.x + (size * i), position.y, size, size}, 0.0f, 13);
		img->drawDepth = depth;
		img->SetColor(color.x, color.y, color.w, color.h);
		img->SetTextureData(rect.x, rect.y, rect.w, rect.h, 0.0f);
		if (staticImage)
			img->SetTransformType(n_TransformTypes::TRANSFORM_STATIC);
		images.push_back(img);
	}
}

Number::~Number()
{
	for (Image *i : images)
		delete i;
}
