#pragma once
#include "../config.h"
#include "objectLoader.h"

struct MeshCreateInfo
{
	const char* filename;
	glm::mat4 preTransform;
};

class ObjectMesh
{
public:
	unsigned int VBO, VAO, vertexCount;

	ObjectMesh(MeshCreateInfo* createInfo);
	~ObjectMesh();	
};