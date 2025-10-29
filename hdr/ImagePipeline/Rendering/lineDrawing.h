
#ifndef LINES_H
# define LINES_H

# include "box.h"
# include <glm/vec4.hpp>

void InitLines();
void DrawLinesWithColor(std::vector<t_Point> &points, glm::vec4 color);

#endif
