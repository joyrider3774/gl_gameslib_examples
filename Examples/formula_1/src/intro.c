#include <stdint.h>
#include <gl_gamelib.h>
#include "commonvars.h"
#include "printfuncs.h"
#include "intro.h"

#define FRAMEDELAY 60


int ay;

void initIntro(void)
{
    ay = gl_WindowHeight;
}

void intro(void)
{
    if (gameState == gsInitIntro)
    {
        initIntro();
        gameState -= gsInitDiff;
    }
    
    frames++;
    gl_clearScreen(makeColor(0, 0, 0, 255));
    if (frames < FRAMEDELAY)
    {
        printMessage(introFont, (14 - 12) >> 1, 4, 24, "WILLEMS DAVY");
    }
    else
    {
        if (frames < FRAMEDELAY *2)
        {
            printMessage(introFont, (14-8) >> 1, 4, 24, "PRESENTS");
        }
        else
        {
            if(ay > 0)
            {
                ay -= 10;
            }
            else
            {
                gameState = gsInitGameIntro;
            }
        }
    }
        
    if (gl_buttonJustPressed(ButA) || gl_buttonJustPressed(ButB))
    {            
        gameState = gsInitGameIntro;
    }

    if (gl_buttonJustPressed(ButUp))
    {
        debugMode = 1;
    }
}
