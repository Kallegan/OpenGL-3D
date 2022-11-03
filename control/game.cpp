#include "game.h"

Game::Game(GameCreateInfo* createInfo)
{
	this->width = createInfo->width;  
	this->centerXMouse = createInfo->width / 2;
	this->height = createInfo->height;
	this->centerYMouse = createInfo->height / 2;

	//seconds since program started.
	lastTime = glfwGetTime();
	numFrames = 0;
	frameTime = 16.0f;

	window = makeWindow();
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	renderer = new Engine(width, height);
	scene = new Scene();	
}



GLFWwindow* Game::makeWindow()
{
	glfwInit(); //initialize glfw

	//OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //version x..
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5); //version ..x
	//quicker than old profile since it doesn't lock main while processing GPU.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create the window for displaying.
	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create window!\n";
		return NULL;
	}
	//sets window as current to display.
	glfwMakeContextCurrent(window);

	//cast using gladload to get all function pointers for OpenGL.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD\n";
		return NULL;
	}

	glViewport(0, 0, width, height);

	return window;
}


returnCode Game::gameLoop()
{
	//input
	returnCode nextAction{ ProcessInput() };
	//clear input events
	glfwPollEvents();

	//update
	scene->update(frameTime / 16.0f);

	//draw
	renderer->render(scene);
	
	glfwSwapBuffers(window);
	calculateFPS();
	return nextAction;
}

Game::~Game()
{
	//cleanup
	delete scene;
	delete renderer;
	glfwTerminate();
}



returnCode Game::ProcessInput()
{
	returnCode nextAction{ returnCode::CONTINUE };

	int wasdState{ 0 };
	float walkDirection{ scene->player->eulers.z };
	bool bWalking{ false };
	float moveSpeed = 1.f;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		wasdState += 1;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		wasdState += 2;

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		wasdState += 4;

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		wasdState += 8;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		return returnCode::QUIT;		


	switch (wasdState)
	{
	case 1:
	case 11:
		//forwards
		bWalking = true;
		break;
	case 3:
		//left-forwards
		bWalking = true;
		walkDirection += 45;
		break;
	case 2:
	case 7:
		//left
		bWalking = true;
		walkDirection += 90;
		break;
	case 6:
		//left-backwards
		bWalking = true;
		walkDirection += 135;
		break;
	case 4:
	case 14:
		//backwards
		bWalking = true;
		walkDirection += 180;
		break;
	case 12:
		//right-backwards
		bWalking = true;
		walkDirection += 225;
		break;
	case 8:
	case 13:
		//right
		bWalking = true;
		walkDirection += 270;
		break;
	case 9:
		//right-forwards
		bWalking = true;
		walkDirection += 315;
	}

	//player movement
	if (bWalking)
	{
		scene->movePlayer(
			moveSpeed * frameTime / 16.0f * glm::vec3
			{
				glm::cos(glm::radians(walkDirection)),
				glm::sin(glm::radians(walkDirection)),
				0.0f
			});
	}

	//mouse controller
	double mouseX, mouseY;
	float mouseSpeed = 0.1f;
	//passing ref so cursorpos can be set from glfw function.
	glfwGetCursorPos(window, &mouseX, &mouseY);
	glfwSetCursorPos(window, centerXMouse, centerYMouse);

	//horizontal rotation
	float deltaX{ static_cast<float>(mouseX - centerXMouse) };
	//vertical rotation
	float deltaY{ static_cast<float>(mouseY - centerYMouse) };
	scene->spinPlayer(frameTime / 16.0f * glm::vec3
		{
			0.0f, deltaY, -deltaX 
		}
	);




	return returnCode::CONTINUE;
}

void Game::calculateFPS()
{
	//get current time since program start (seconds).
	currentTime = glfwGetTime();
	//subtract current with last to know time passed since last update.
	double delta = currentTime - lastTime;

	
	if (delta >= 1)
	{
		int framerate{ std::max(1, int(numFrames / delta)) };
		std::stringstream title;
		title << "FPS: " << framerate;
		glfwSetWindowTitle(window, title.str().c_str());
		lastTime = currentTime;
		numFrames = -1;
		frameTime = float(100.0 / framerate);
	}

	++numFrames;


}

