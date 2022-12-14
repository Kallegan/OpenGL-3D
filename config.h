#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <array>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include "tiny_obj_loader.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <sstream>
#include <fstream>
#include <string>

struct image
{
	unsigned char* pixels;
	int width, height, channels;
};