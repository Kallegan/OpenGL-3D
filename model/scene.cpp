#include "scene.h"



Scene::Scene()
{
	//create playerinfo class.
	PlayerCreateInfo playerInfo;
	//pass data to playerinfo.
	playerInfo.eulers = { 0.0f, 90.0f, 0.0f };
	playerInfo.position = { 0.0f, 0.0f, 1.0f };
	//create new player with the data from current playerinfo.
	player = new Player(&playerInfo);

	CubeCreateInfo cubeInfo;
	cubeInfo.eulers = { 0.0f, 0.0f, 0.0f };
	cubeInfo.positions = { 3.0f, 0.0f, 0.5f };
	cube = new Cube(&cubeInfo);
}

Scene::~Scene()
{
	delete cube;
	delete player;
}

void Scene::update(float rate)
{
	player->update();
	cube->update(1.0f);
}

void Scene::movePlayer(glm::vec3 dPos)
{
	player->position += dPos;
}

void Scene::spinPlayer(glm::vec3 dEulers)
{
	player->eulers += dEulers;

	if (player->eulers.z < 0)
		player->eulers.z += 360;

	else if (player->eulers.z > 360)
		player->eulers.z -= -360;

	//sets y rotation to clamp up / down based on look angle.
	player->eulers.y = std::max(std::min(player->eulers.y, 179.0f), 1.0f);
}
