#pragma once
#include "../config.h"
#include "../model/scene.h"
#include "shader.h"
#include "rectangleModel.h"
#include "objectMesh.h"
#include "material.h"

class Engine
{
public:
	Engine(int width, int height);
	~Engine();

	void createMaterials();
	void createModels();
	void render(Scene* scene);

	unsigned int shader;
	Material* cardboardMaterial;	 
	ObjectMesh* cubeModel;
};