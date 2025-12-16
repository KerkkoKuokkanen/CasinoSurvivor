
#ifndef NUMBER_H
# define NUMBER_H

# include "image.h"

class Number
{
	private:
		std::vector<Image*> images;
	public:
		int num = 0;
		Number(t_Point position, std::string number, float size, t_Box color, float depth);
		~Number();
};

#endif
