#include <gl_gamelib.h>
#include <stdbool.h>

int mainLoop(void* ud)
{	
	bool inverted = true;
	gl_startFrame();
	gl_inputUpdate();
	gl_endFrame();
	return 1;
}

void setupGame()
{	
	gl_platformInit();
	gl_graphicsInit(400, 240, 30, "test", false, true, makeColor(0,0,0,255));
}


#ifdef PLATFORM_PLAYDATE

#ifdef _WINDLL
__declspec(dllexport)
#endif

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