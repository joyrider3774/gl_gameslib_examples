#include <stdint.h>
#include <string.h>
#include <gl_gamelib.h>
#include "commonvars.h"
#include "sound.h"

int sound_on = 0;
int tickSound, crashSound;
void setSoundOn(int value)
{
    sound_on = value;
}


int isSoundOn(void)
{
    return sound_on;
}

void initSound(void)
{
    gl_AudioInit();
    tickSound = gl_AudioLoadSound("sound/tick.wav");
    crashSound = gl_AudioLoadSound("sound/crash.wav");
}

void deinitSound(void)
{
    gl_AudioUnLoadSound(tickSound);
    gl_AudioUnLoadSound(crashSound);
    gl_AudioDeInit();
}

void playTickSound(void)
{
    if(!sound_on)
    {
        return;
    }
    gl_AudioPlaySound(tickSound,0);
}

void playCrashSound(void)
{
    if(!sound_on)
    {
        return;
    }
    gl_AudioPlaySound(crashSound,0);
}
