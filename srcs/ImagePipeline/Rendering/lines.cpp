
#include "lines.h"
#include "lineDrawing.h"

Lines::Lines(int layer)
{
	AddToRenderSystem(layer);
	drawY = 10000.0f;
	drawDepth = 10000.0f;
}

void Lines::SetLines(std::vector<t_Point> &lines)
{
	Lines::lines.clear();
	for (int i = 0; i < lines.size(); i++)
		Lines::lines.push_back(lines[i]);
}

void Lines::SetColor(float r, float g, float b, float a)
{
	color = {r, g, b, a};
}

void Lines::Draw()
{
	if (active == false)
		return ;
	DrawLinesWithColor(lines, color);
}
