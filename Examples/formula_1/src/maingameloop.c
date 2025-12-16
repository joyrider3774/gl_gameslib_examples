#include <stdlib.h>
#include <stdio.h>
#include <gl_gamelib.h>
#include "maingameloop.h"
#include "commonvars.h"
#include "intro.h"
#include "sound.h"
#include "game.h"
#include "savestate.h"

void preloadImages(void)
{
    introFont = gl_loadImage("images/bigfont.png",1,41,alternateSource);
    lcdFont = gl_loadImage("images/lcdfont.png",1,41,alternateSource);
    Player = gl_loadImage("images/player.png",1,1,alternateSource);
    Enemy = gl_loadImage("images/enemy.png",1,1,alternateSource);
    Background = gl_loadImage("images/background.png",1,1,alternateSource);
}

void unLoadImages(void)
{
	gl_freeImage(introFont);
	gl_freeImage(lcdFont);
	gl_freeImage(Player);
	gl_freeImage(Enemy);
	gl_freeImage(Background);
}

void resetGlobals()
{
	frames = 0;
	for (u8 X = 0; X < 3; X++)
		for (u8 Y = 0; Y < 3; Y++)
			EnemyStates[X][Y] = false;
	for (u8 X = 0; X < 3; X++)
		PlayerStates[X] = false;
	PlayerStates[1] = false;
    alternateSource = 1;
}

// game initialization
void setupGame(void)
{  
    resetGlobals();
	gl_platformInit();
	gl_graphicsInit(320, 240, FRAMERATE, "Formula 1", false, true, makeColor(0,0,0,255));
    gameState = gsInitIntro;
    preloadImages();    
    initSaveState();    
    initSound();
    setSoundOn(1);   
}

void terminateGame(void)
{
	unLoadImages();
	deinitSound();
}

// main update function
int mainLoop(void* ud)
{
    gl_startFrame();
    gl_inputUpdate();

    //gamestate handling   
    switch (gameState)
    {
        case gsInitIntro:
        case gsIntro:
            intro();
            break;
        case gsInitGame:
        case gsGame:
            game();
            break;
        case gsInitGameIntro:
        case gsGameIntro:
            gameIntro();
            break;
        case gsInitGameOver:
        case gsGameOver:
            gameOver();
            break;
        default:
            break;
    }
    // if (debugMode)
    // {
    //     pd->system->drawFPS(0, 0);
    // }
    gl_endFrame();
	return 1;
}
