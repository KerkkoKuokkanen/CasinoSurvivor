
#ifndef NUMBER_H
# define NUMBER_H

# include "image.h"

class Number
{
	private:
		std::vector<Image*> images;
		t_Point position = {0.0f, 0.0f};
		float size = 0.0f;
	public:
		int num = 0;
		Number(t_Point position, std::string number, float size, t_Box color, float depth);
		~Number();
		void SetColor(float r, float g, float b, float a);
		void SetPosition(t_Point pos);
		t_Point GetPosition() {return (position);};
};

#endif
