#include "engine.h"

Engine::Engine(int widht, int height)
{
	shader = util::loadShader("shaders/vertex.txt", "shaders/fragment.txt");
	glUseProgram(shader);
	//allocating texture 0 to the texture.
	glUniform1i(glGetUniformLocation(shader, "basicTexture"), 0);


	float aspectRatio = (float)widht / (float)height;

	//setup frambuffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //what color to clear screen with.
	glEnable(GL_DEPTH_TEST);
	//setup perspective transform for the shader.
	glm::mat4 projectionTransform = glm::perspective(45.f, aspectRatio, 0.1f, 10.0f);
	//4floatvector matrix,sends projection data to shader.
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projectionTransform));

	lights.colorLoc = glGetUniformLocation(shader, "light.color");
	lights.positionLoc = glGetUniformLocation(shader, "light.position");
	lights.strengthLoc = glGetUniformLocation(shader, "light.strength");

	cameraPosLoc = glGetUniformLocation(shader, "cameraPosition");

	createModels();
	createMaterials();	
} 

Engine::~Engine()
{
	delete cardboardMaterial;
	delete cubeModel;
	glDeleteProgram(shader);
}

void Engine::createModels()
{
	MeshCreateInfo cubeInfo;
	cubeInfo.filename = "models/cube.obj";
	cubeInfo.preTransform = 1.f * glm::mat4(1.0);
	cubeModel = new ObjectMesh(&cubeInfo);
}

void Engine::createMaterials()
{
	MaterialCreateInfo materialInfo;
	materialInfo.filename = "textures/wood.jpg";
	cardboardMaterial = new Material(&materialInfo);
}

void Engine::render(Scene* scene)
{
	//prepare shaders
	glm::mat4 viewTransform{ glm::lookAt(
		scene->player->position,
		scene->player->position + scene->player->forwards,
		scene->player->up) };
	//set view in shader
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"),
		1, GL_FALSE, glm::value_ptr(viewTransform));

	//update transform
	glm::mat4 modelTransform = { glm::mat4(1.0f) };
	modelTransform = glm::translate(modelTransform, scene->cube->position);

	modelTransform = modelTransform * glm::eulerAngleXYZ
	(
		scene->cube->eulers.x, scene->cube->eulers.y, scene->cube->eulers.z
	);

	glUniformMatrix4fv(glGetUniformLocation(shader, "model"),
		1, GL_FALSE, glm::value_ptr(modelTransform));

	glUniform3fv(cameraPosLoc, 1, glm::value_ptr(scene->player->position));

	Light* light = scene->lights[0];
	glUniform3fv(lights.colorLoc, 1, glm::value_ptr(light->color));
	glUniform3fv(lights.positionLoc, 1, glm::value_ptr(light->position));
	glUniform1f(lights.strengthLoc, light->strength);


	//draw		
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear buffer.
	glUseProgram(shader); //setup shader program.
	cardboardMaterial->use();
	//binds to texture unit declared above with loaded texture.
	glBindVertexArray(cubeModel->VAO);
	glDrawArrays(GL_TRIANGLES, 0, cubeModel->vertexCount);
}
