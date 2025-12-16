#include "commonvars.h"
#include <stdbool.h>
#include <gl_gamelib.h>

u8 gameState, debugMode;

gl_Image* Player, * Enemy, * Background;
gl_Image* introFont, * lcdFont;

char* verifyCode = NULL;
bool EnemyStates[3][3];
bool PlayerStates[3];
u8 HitPosition, LivesLost;
u16 Teller, FlashesDelay, Flashes, Delay;
u32 Score;
bool CanMove, CrashSoundPlayed;
u32 frames;
u8 alternateSource;
