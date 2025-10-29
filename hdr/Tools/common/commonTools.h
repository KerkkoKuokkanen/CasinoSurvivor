
#ifndef COMMON_TOOLS_H
# define COMMON_TOOLS_H

# define PI 3.1415926
# include <time.h>
# include "snapshot.h"

float float_rand();
int rounding(float num);
bool FAlmostEqual(float a, float b, float precision = 0.00001f);
uint64_t HashData64(const void *data, size_t length);
uint32_t HashData32(const void *data, size_t length);
uint16_t HashData16(const void *data, size_t length);
bool SystemIsLittleEndian();
SnapShot MakeIntoSnapshot(void *data);

#endif
