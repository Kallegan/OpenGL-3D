#include "image.h"

image util::loadFromFile(const char* filename)
{	
	image result;

	result.pixels = stbi_load(filename, &(result.width), &(result.height), &(result.channels), STBI_rgb_alpha);

	return result;
}

void util::freeImgMem(image garbageImg)
{
	//free the data after setting all up.
	stbi_image_free(garbageImg.pixels);
}

