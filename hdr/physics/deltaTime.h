
#ifndef DELTA_TIME_H
# define DELTA_TIME_H

void SetDeltaTime(float time);

//Delta time that can be modified
float DeltaTime();
void InitDeltaTime(unsigned int frameRate);

//The delta time that is never modified
float DeltaTimeReal();
void SetDeltaMulti(float mult);

#endif
