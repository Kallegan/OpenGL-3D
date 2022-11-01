#include "engine.h"

Engine::Engine(int widht, int height)
{
	shader = util::loadShader("shaders/vertex.txt", "shaders/fragment.txt");
	glUseProgram(shader);
	//allocating texture 0 to the texture.
	glUniform1i(glGetUniformLocation(shader, "basicTexture"), 0);


	float aspectRatio = (float)widht / (float)height;

	//setup frambuffer
	glClearColor(0.2f, 0.2f, 0.3f, 1.0f); //what color to clear screen with.
	glEnable(GL_DEPTH_TEST);
	//setup perspective transform for the shader.
	glm::mat4 projectionTransform = glm::perspective(45.0f, aspectRatio, 0.1f, 10.0f);
	//4floatvector matrix,sends projection data to shader.
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projectionTransform));

	createModels();
	createMaterials();	
} 

Engine::~Engine()
{
	delete cardboardMaterial;
	delete cubeModel;
	glDeleteProgram(shader);
}

void Engine::createMaterials()
{
	MaterialCreateInfo materialInfo;
	materialInfo.filename = "textures/cardboard.jpg";
	cardboardMaterial = new Material(&materialInfo);
}

void Engine::createModels()
{
	RectangleModelCreateInfo cubeInfo;
	cubeInfo.size = { 1.0f, 1.0f, 1.0f };
	cubeModel = new RectangleModel(&cubeInfo);
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

	//draw		
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear buffer.
	glUseProgram(shader); //setup shader program.
	cardboardMaterial->use();
	//binds to texture unit declared above with loaded texture.
	glBindVertexArray(cubeModel->VAO);
	glDrawArrays(GL_TRIANGLES, 0, cubeModel->vertexCount);
}
