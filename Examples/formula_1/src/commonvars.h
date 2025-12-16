#ifndef commonvars_h
#define commonvars_h

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <gl_gamelib.h>

#define FRAMERATE 50

#define gsGame 0
#define gsIntro 1
#define gsGameIntro 2
#define gsGameOver 3

#define gsInitDiff 50

#define gsInitGame (gsInitDiff + gsGame)
#define gsInitIntro (gsInitDiff + gsIntro)
#define gsInitGameIntro (gsInitDiff + gsGameIntro)
#define gsInitGameOver (gsInitDiff + gsGameOver)
extern char* verifyCode;
extern u8 gameState, debugMode;
extern bool EnemyStates[3][3];
extern bool PlayerStates[3];
extern u8 HitPosition, LivesLost;
extern u16 Teller, FlashesDelay, Flashes, Delay;
extern bool CanMove, CrashSoundPlayed;


extern gl_Image *Player, *Enemy, *Background;
extern gl_Image* introFont, *lcdFont;
extern u32 frames;
extern u8 alternateSource;
extern u32 Score;
#endif