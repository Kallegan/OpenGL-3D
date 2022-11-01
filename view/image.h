#pragma once
#include "../config.h"


namespace util
{
	image loadFromFile(const char* filename);

	void freeImgMem(image garbageImg);
	
}
