
#ifndef LETTER_TABLE_H
# define LETTER_TABLE_H

# include "box.h"

std::tuple<t_Box, t_Point> LetterTable(char c, std::string font);
void InitFontTable();
float GetFontMaxWidth(std::string font);

#endif
