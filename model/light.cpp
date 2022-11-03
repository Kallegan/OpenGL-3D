#include "light.h"

Light::Light(LightCreateInfo* createInfo)
{
	this->position = createInfo->positions;
	this->color = createInfo->color;
	this->strength = createInfo->strength;
}
