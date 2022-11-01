#include "config.h"
#include "control/game.h"

int main()
{		
	int width = 640;
	int height = 480;
	int mouseXStart = width / 2;
	int mouseYStart = height / 2;
	
	GameCreateInfo appInfo;
	appInfo.width = width;
	appInfo.height = height;
	Game* app = new Game(&appInfo);


	Engine* renderer = new Engine(width, height);

	returnCode nextAction = returnCode::CONTINUE;
	while (nextAction == returnCode::CONTINUE)
	{
		nextAction = app->gameLoop();
	}

	delete app;
	
	
	glfwTerminate();	
	return 0;	
}