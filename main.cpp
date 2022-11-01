#include "config.h"
#include "view/shader.h"
#include "model/scene.h"
#include "view/RectangleModel.h"
#include "view/material.h"


void processInput(GLFWwindow* window, Scene* scene, int halfWidht, int halfHeight)
{
	int wasdState{ 0 };
	float walkDirection{ scene->player->eulers.z};
	bool bWalking{ false };
	float moveSpeed = 0.1f;
	
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		wasdState += 1;
	
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		wasdState += 2;

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)	
		wasdState += 4;
	
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)	
		wasdState += 8;
		
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);


	switch (wasdState) 
	{
	case 1:
	case 11:
		//forwards
		bWalking = true;
		break;
	case 3:
		//left-forwards
		bWalking = true;
		walkDirection += 45;
		break;
	case 2:
	case 7:
		//left
		bWalking = true;
		walkDirection += 90;
		break;
	case 6:
		//left-backwards
		bWalking = true;
		walkDirection += 135;
		break;
	case 4:
	case 14:
		//backwards
		bWalking = true;
		walkDirection += 180;
		break;
	case 12:
		//right-backwards
		bWalking = true;
		walkDirection += 225;
		break;
	case 8:
	case 13:
		//right
		bWalking = true;
		walkDirection += 270;
		break;
	case 9:
		//right-forwards
		bWalking = true;
		walkDirection += 315;
	}

	//player movement
	if (bWalking)
	{
		scene->movePlayer(
		moveSpeed * glm::vec3
		{
			glm::cos(glm::radians(walkDirection)),
			glm::sin(glm::radians(walkDirection)),
			0.0f
		});
	}

	//mouse controller
	double mouseX, mouseY;
	float mouseSpeed = 0.1f;
	//passing ref so cursorpos can be set from glfw function.
	glfwGetCursorPos(window, &mouseX, &mouseY);
	glfwSetCursorPos(window, halfWidht, halfHeight);

	//horizontal rotation
	float deltaX{ static_cast<float>(mouseX - halfWidht) };
	//vertical rotation
	float deltaY{ static_cast<float>(mouseY - halfHeight) };
	scene->spinPlayer(
		{
			0.0f, deltaY * mouseSpeed, - deltaX * mouseSpeed
		}
	);
}

GLFWwindow* InitWindow(int width, int height)
{
	glfwInit(); //initialize glfw

	//OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //version x..
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5); //version ..x
	//quicker than old profile since it doesn't lock main while processing GPU.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create the window for displaying.
	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create window!\n";
		return NULL;
	}
	//sets window as current to display.
	glfwMakeContextCurrent(window);

	//cast using gladload to get all function pointers for OpenGL.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD\n";		
		return NULL;
	}

	glViewport(0, 0, width, height);

	return window;
}



int main()
{	
	Scene* scene = new Scene();
	
	int width = 640;
	int height = 480;
	float aspectRatio = (float)width / (float)height;

	GLFWwindow* window = InitWindow(width, height);	
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	
	unsigned int shader = util::loadShader("shaders/vertex.txt", "shaders/fragment.txt");
	glUseProgram(shader);
	
	RectangleModelCreateInfo cubeInfo;
	cubeInfo.size = { 1.0f, 1.0f, 1.0f };
	RectangleModel* cubeModel = new RectangleModel(&cubeInfo);
		
	//allocating texture 0 to the texture.
	glUniform1i(glGetUniformLocation(shader, "basicTexture"), 0);

	MaterialCreateInfo materialInfo;
	materialInfo.filename = "textures/cardboard.jpg";
	Material* cardboardMaterial = new Material(&materialInfo);

	//setup frambuffer
	glClearColor(0.2f, 0.2f, 0.3f, 1.0f); //what color to clear screen with.
	glEnable(GL_DEPTH_TEST);
	//setup perspective transform for the shader.
	glm::mat4 projectionTransform = glm::perspective(45.0f, aspectRatio, 0.1f, 10.0f);
	//4floatvector matrix,sends projection data to shader.
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projectionTransform));

	int halfWidth = width / 2;
	int halfHeight = height / 2;

	while (!glfwWindowShouldClose(window))
	{	
		//handles window input.
		processInput(window, scene, halfWidth, halfHeight);
		//flushes queue events.
		glfwPollEvents();

		scene->update(1.0f);
		
	
		
		//prepare shaders
		glm::mat4 viewTransform { glm::lookAt(
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
	
		//swaps buffer to avoid tearing / sync issues that happens when displaying while drawing.
		glfwSwapBuffers(window);
	}

	//free mem.
	delete cubeModel;
	delete scene;
	delete cardboardMaterial;
	glDeleteProgram(shader);
	glfwTerminate();	
	return 0;	
}