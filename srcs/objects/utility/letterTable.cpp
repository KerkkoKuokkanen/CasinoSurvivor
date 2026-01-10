
#include "box.h"
#include <unordered_map>
#include <string>
#include <stdio.h>

struct FileData{
	float uStart, vStart;
	float uEnd, vEnd;
	float width, height;
	int xOffset, yOffset;
	int xAdvance;
	int active;
};

std::unordered_map<std::string, std::unordered_map<int, FileData>> fonts;
std::unordered_map<std::string, float> biggestSize;

void LoadFontMetadata(const char* filepath, std::unordered_map<int, FileData> &fontData, std::string num)
{
	FILE* file = fopen(filepath, "r");
	if (!file)
		return;

	char line[256];
	float wMax = -10.0f;
	while (fgets(line, sizeof(line), file)) {
		int id;
		float us, vs, ue, ve, w, h;
		int xo, yo, xa;

		// sscanf template matching your exact format:
		// #charId=33 #uvStart=(0.9750, 0.4500) #uvEnd=(1.0000, 0.6000) #size=(8.0, 48.0) #xOffset=3 #yOffset=-17 #xAdvance=11
		int found = sscanf(line, 
			"#charId=%d #uvStart=(%f, %f) #uvEnd=(%f, %f) #size=(%f, %f) #xOffset=%d #yOffset=%d #xAdvance=%d",
			&id, &us, &vs, &ue, &ve, &w, &h, &xo, &yo, &xa);

		if (found == 10) { // Ensure all 10 variables were filled
			fontData[id].uStart = us;
			fontData[id].vStart = vs;
			fontData[id].uEnd = ue;
			fontData[id].vEnd = ve;
			fontData[id].width = w;
			fontData[id].height = h;
			fontData[id].xOffset = xo;
			fontData[id].yOffset = yo;
			fontData[id].xAdvance = xa;
			fontData[id].active = 1;
			if (w > wMax)
				wMax = w;
		}
	}
	biggestSize[num] = wMax;
	fclose(file);
}

float GetFontMaxWidth(std::string font)
{
	return (biggestSize[font]);
}

std::tuple<t_Box, t_Point> LetterTable(char c, std::string font)
{
	t_Box b;
	t_Point p;
	b.x = fonts[font][c].uStart;
	b.y = fonts[font][c].vStart;
	b.w = fonts[font][c].uEnd;
	b.h = fonts[font][c].vEnd;
	p.x = fonts[font][c].width;
	p.y = fonts[font][c].height;
	return {b, p};
}

void InitFontTable()
{
	LoadFontMetadata("assets/sets/fontMetaData/font0.fnt", fonts["font0"], "font0");
	LoadFontMetadata("assets/sets/fontMetaData/font1.fnt", fonts["font1"], "font1");
	LoadFontMetadata("assets/sets/fontMetaData/font2.fnt", fonts["font2"], "font2");
	LoadFontMetadata("assets/sets/fontMetaData/font3.fnt", fonts["font3"], "font3");
	LoadFontMetadata("assets/sets/fontMetaData/font4.fnt", fonts["font4"], "font4");
	LoadFontMetadata("assets/sets/fontMetaData/outline0.fnt", fonts["outline0"], "outline0");
	LoadFontMetadata("assets/sets/fontMetaData/outline1.fnt", fonts["outline1"], "outline1");
	LoadFontMetadata("assets/sets/fontMetaData/bold.fnt", fonts["bold"], "bold");
}
