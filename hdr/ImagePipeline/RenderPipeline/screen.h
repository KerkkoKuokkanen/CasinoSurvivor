
#ifndef SCREEN_H
# define SCREEN_H

extern int __currentScreenWidth;
extern int __currentScreenHeight;
extern unsigned int __currentScreenFrameRate;
extern bool __forceAspectRatio;

float GetHeightMinus();
float GetWidthMinus();
void SetFrameRate(unsigned int frameRate);
unsigned int GetFrameRate();

#endif
