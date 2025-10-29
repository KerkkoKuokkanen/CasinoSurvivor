
#include "textImage.h"
#include "imgui.h"
#include "imageTransforms.h"

TextImage::TextImage(std::string text, t_Point pos, int layer)
{
	position = pos;
	TextImage::text = text;
	AddToRenderSystem(layer);
}

void TextImage::SetRBGA(int r, int g, int b, int a)
{
	TextImage::r = r;
	TextImage::g = g;
	TextImage::b = b;
	TextImage::a = a;
}

void TextImage::SetFontSize(int size)
{
	if (size < 0)
		return ;
	if (size > 12)
		return ;
	fontSize = size;
}

void TextImage::Draw()
{
	const char *used = text.c_str();

	ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
	t_iPoint pos = TransformToPixelCoordinates(position.x, position.y);

	ImGuiIO& io = ImGui::GetIO();
	ImFont* largeFont = io.Fonts->Fonts[fontSize]; // Index 1 for the second font loaded

	ImGui::PushFont(largeFont);
	draw_list->AddText(ImVec2(pos.x, pos.y), IM_COL32(r, g, b, a), used);
	ImGui::PopFont();
}

void TextImage::ChangeText(std::string text)
{
	TextImage::text = text;
}
