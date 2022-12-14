#include "Cube.h"

Cube::Cube(CubeCreateInfo* createInfo)
{
	this->position = createInfo->positions;
	this->eulers = createInfo->eulers;
}

void Cube::update(float rate)
{
	eulers.x += 0.0001 * rate;
	if (eulers.x > 360)
		eulers.x -= 360;

	eulers.y += 0.0002 * rate;
	if (eulers.y > 360)
		eulers.y -= 360;

	modelTransform = glm::mat4(1.0f);
	modelTransform = glm::translate(modelTransform, position);
	modelTransform = modelTransform * glm::eulerAngleXYZ(eulers.x, eulers.y, eulers.z);
}
