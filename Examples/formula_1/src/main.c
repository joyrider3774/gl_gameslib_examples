#include <stdio.h>
#include <stdlib.h>
#include <gl_gamelib.h>
#include "commonvars.h"
#include "maingameloop.h"

#ifdef _WINDLL
__declspec(dllexport)
#endif



#ifdef PLATFORM_PLAYDATE
int eventHandler(PlaydateAPI* playdate, PDSystemEvent event, uint32_t arg)
{

	if ( event == kEventInit )
	{	
		gl_setPlaydateApi(playdate);
		setupGame();
		playdate->system->setUpdateCallback(mainLoop, NULL);
	}

	return 0;
}
#else

int main(int argc, char **argv)
{
	setupGame();
	bool quit = false;
	while(!quit)
	{	
		quit = gl_buttonPressed(ButQuit);
		mainLoop(NULL);
	}
}
#endif