#pragma once
#include "../config.h"
#include "cube.h"
#include "player.h"

//scene has access to all objects, like ue levels. When we update objects, its done via scene.
class Scene
{
public:
	Scene();
	~Scene();
	void update(float rate);
	void movePlayer(glm::vec3 dPos);
	void spinPlayer(glm::vec3 dEulers);

	Cube* cube;
	Player* player;
};