
#include <time.h>
#include <string.h>
#include <time.h>
#include <gl_gamelib.h>
#include "commonvars.h"
#include "sound.h"
#include "printfuncs.h"
#include "savestate.h"


void DrawGame(void)
{
	int X, Y;
	for (X = 0; X < 3; X++)
		for (Y = 0; Y < 3; Y++)
			if (EnemyStates[X][Y])
			{
				gl_drawImage(Enemy, 90 + (X * 54), 48 + (Y * 47), 1);
			}

	for (X = 0; X < 3; X++)
	{
		if (PlayerStates[X])
		{
			gl_drawImage(Player, 90 + (X * 54), 180, 1);
		}
	}
}

void MoveEnemy(void)
{
	int X, Y;
	for (X = 0; X < 3; X++)
		for (Y = 2; Y >= 1; Y--)
			EnemyStates[X][Y] = EnemyStates[X][Y - 1];
	for (X = 0; X < 3; X++)
		EnemyStates[X][0] = false;
	for (X = 0; X <= 1; X++)
		EnemyStates[rand() % 3][0] = true;
}

void MoveLeft(void)
{
	int X;
	for (X = 0; X < 2; X++)
	{
		if (PlayerStates[X + 1])
		{
			PlayerStates[X] = true;
			PlayerStates[X + 1] = false;
		}
	}
}

void MoveRight(void)
{
	int X;
	for (X = 2; X >= 1; X--)
	{
		if (PlayerStates[X - 1])
		{
			PlayerStates[X] = true;
			PlayerStates[X - 1] = false;
		}
	}
}

bool IsCollided(void)
{
	int X;
	bool Temp;
	Temp = false;
	for (X = 0; X < 3; X++)
		if (PlayerStates[X] && EnemyStates[X][2])
		{
			Temp = true;
			HitPosition = X;
		}
	return Temp;
}

void HitFlash(void)
{
	PlayerStates[HitPosition] = !PlayerStates[HitPosition];
	EnemyStates[HitPosition][2] = !EnemyStates[HitPosition][2];
}

void InitialiseStates(void)
{
	int X, Y;
	for (X = 0; X < 3; X++)
		for (Y = 0; Y < 3; Y++)
			EnemyStates[X][Y] = false;
	for (X = 0; X < 3; X++)
		PlayerStates[X] = false;
	PlayerStates[1] = true;
}

void initGame(void)
{
#ifdef PLATFORM_PLAYDATE	
	srand(pd->system->getCurrentTimeMilliseconds());
#else
	srand(time(NULL));
#endif
	Teller = 25;
	FlashesDelay = 14;
	Flashes = 0;
	CanMove = true;
	Score = 0;
	Delay = 60;
	LivesLost = 0;
	CrashSoundPlayed = false;
	InitialiseStates();
}


void game(void)
{
	int X;
	gl_clearScreen(makeColor(0, 0, 0, 255));
    if (gameState == gsInitGame)
    {
        initGame();
        gameState -= gsInitDiff;
    }
    
   
    if (gl_buttonJustPressed(ButRight))
    {
		if (CanMove)
			MoveRight();
    }

    if (gl_buttonJustPressed(ButLeft))
    {
		if (CanMove)
			MoveLeft();
    }

	gl_drawImage(Background, 0, 0, 1);
	printNumber(lcdFont, 24, 4, 8, Score, 6);
	printNumber(lcdFont, 17, 4, 8, getHiScore(), 6);
	if (LivesLost >= 1)
	{
		for (X = 0; X < LivesLost; X++)
			printMessage(lcdFont, 11 + X, 4, 8, "X");
	}
	Teller++;
	if (Teller >= Delay)
	{
		if (!IsCollided() && CanMove)
		{
			Teller = 0;
			for (X = 0; X < 3; X++)
				if (EnemyStates[X][2])
				{
					Score += 10;
					if (Score > getHiScore())
						setHiScore(Score);
					if ((Score % 100 == 0) && (Delay > 16))
						Delay--;
				}
			MoveEnemy();
			playTickSound();
		}
		else
		{
			if (!CrashSoundPlayed)
			{
				playCrashSound();
				CrashSoundPlayed = true;
			}
			CanMove = false;
			FlashesDelay++;
			if (FlashesDelay == 15)
			{
				Flashes++;
				HitFlash();
				FlashesDelay = 0;
				if (Flashes == 6)
				{
					Flashes = 0;
					CanMove = true;
					Teller = 0;
					CrashSoundPlayed = false;
					EnemyStates[HitPosition][2] = false;
					LivesLost++;
					FlashesDelay = 14;
					if (LivesLost == 3)
						gameState = gsInitGameOver;
				}
			}
		}
	}
	DrawGame();
}

void initGameOver(void)
{

}

void gameOver(void)
{
	u8 X;
	gl_drawFilledRectangle(0,0,gl_WindowWidth, gl_WindowHeight, makeColor(0,0,0,255), false);
	if (gameState == gsInitGameOver)
	{
		initGameOver();
		gameState -= gsInitDiff;
	}

	if (gl_buttonJustPressed(ButRight) || gl_buttonJustPressed(ButLeft) ||
		gl_buttonJustPressed(ButA) || gl_buttonJustPressed(ButB))
	{
		gameState = gsInitGame;
	}


	gl_drawImage(Background, 0, 0, 1);
	printNumber(lcdFont, 24, 4, 8, Score, 6);
	printNumber(lcdFont, 17, 4, 8, getHiScore(), 6);
	if (LivesLost >= 1)
	{
		for (X = 0; X < LivesLost; X++)
			printMessage(lcdFont, 11 + X, 4, 8, "X");
		DrawGame();		
	}
}

void FlashIntro(void)
{
	u8 X, Y;
	for (X = 0; X < 3; X++)
		for (Y = 0; Y < 3; Y++)
			EnemyStates[X][Y] = !EnemyStates[X][Y];
	for (X = 0; X < 3; X++)
		PlayerStates[X] = !PlayerStates[X];
}


void gameIntroInit(void)
{
	LivesLost = 0;
	FlashesDelay = 0;
	Score = 0;
}

void gameIntro(void)
{
	u8 X;
	gl_clearScreen(makeColor(0, 0, 0, 255));
	if (gameState == gsInitGameIntro)
	{
		gameIntroInit();
		gameState -= gsInitDiff;
	}

	if (gl_buttonJustPressed(ButRight) || gl_buttonJustPressed(ButLeft) ||
		gl_buttonJustPressed(ButA) || gl_buttonJustPressed(ButB))
	{
		gameState = gsInitGame;
	}

	gl_drawImage(Background, 0, 0, 1);
	FlashesDelay++;
	if (FlashesDelay == 25)
	{
		FlashesDelay = 0;
		FlashIntro();
		if (PlayerStates[0])
		{
			LivesLost = 3;
			Score = 888888;
		}
		else
		{
			LivesLost = 0;
			Score = 0;
		}
	}
	if (Score > 0)
	{
		printNumber(lcdFont, 24, 4, 8, Score, 6);
		printNumber(lcdFont, 17, 4, 8, Score, 6);
	}
	if (LivesLost >= 1)
	{
		for (X = 0; X < LivesLost; X++)
			printMessage(lcdFont, 11 + X, 4, 8, "X");
	}
	DrawGame();		
}

