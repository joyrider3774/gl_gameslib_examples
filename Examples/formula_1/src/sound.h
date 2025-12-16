#ifndef SOUND_H
#define SOUND_H

#include <stdint.h>


void initSound(void);
void deinitSound(void);
void setSoundOn(int value);
int isSoundOn(void);
void playTickSound(void);
void playCrashSound(void);

#endif