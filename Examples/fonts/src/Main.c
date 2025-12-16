#include <gl_gamelib.h>
#include <stdbool.h>

gl_Image *fontImage = NULL;
gl_SFont_Font *font;
int mainLoop(void* ud)
{	
	gl_startFrame();
	gl_inputUpdate();
	gl_clearScreen(makeColor(255, 255, 255, 255));
	gl_SFont_Write(font, 180, 110, "HELLO FONT");
	gl_endFrame();
	return 1;
}

void setupGame()
{	
	gl_platformInit();
	gl_graphicsInit(400, 240, 30, "test", false, true, makeColor(0,0,0,255));
	fontImage = gl_loadImage("graphics/font_roboto_bold_12.png", 1, 1, 0);
	font = gl_SFont_InitFont(fontImage);
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