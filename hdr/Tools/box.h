
#ifndef BOX_H
# define BOX_H

# define PI 3.1415926

typedef struct s_iPoint
{
	int x;
	int y;
}				t_iPoint;

typedef struct	s_Box
{
	float x;
	float y;
	float w;
	float h;
}				t_Box;

typedef struct	s_Point
{
	float x;
	float y;
}				t_Point;

typedef struct s_BoundingB
{
	t_Point leftTop;
	t_Point rightTop;
	t_Point rightBottom;
	t_Point leftBottom;
}				t_BoundingB;

#endif
