
#ifndef SHAPE_SAVING_H
# define SHAPE_SAVING_H

void SaveNewShape(uint64_t shapeKey, const char *name = "");
uint64_t LoadShape(const char *name);

#endif
