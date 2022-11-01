#pragma once
#include "../config.h"
#include "../model/scene.h"
#include "../view/engine.h"

struct GameCreateInfo
{
	int width;
	int height;
};

enum class returnCode
{
	CONTINUE, QUIT
};

class Game
{
public:
	Game(GameCreateInfo* createInfo);
	~Game();
	returnCode gameLoop();
	

private:
	GLFWwindow* makeWindow();
	returnCode ProcessInput();
	void calculateFPS();

	GLFWwindow* window;
	int width, height, centerYMouse, centerXMouse;
	Scene* scene;
	Engine* renderer;

	double lastTime, currentTime;
	int numFrames;
	float frameTime;
};