
#ifndef TEXT_CLASS_H
# define TEXT_CLASS_H

#include "image.h"

struct Character {
	Image *img;
	char c;
};

class Text
{
	private:
		std::vector<Character> text;
		std::string writing;
	public:
		Text(std::string text, float size, t_Point pos, t_Box color, int layer, float depth);
		~Text();
};

#endif
