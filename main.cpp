#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int main()
{	

	glfwInit(); //initialize glfw

	//OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //version x..
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5); //version ..x
	//quicker than old profile since it doesn't lock main while processing GPU.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create the window for displaying.
	GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create window!\n";
		glfwTerminate();
		return -1;
	}
	//sets window as current to display.
	glfwMakeContextCurrent(window);

	//cast using gladload to get all function pointers for OpenGL.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD\n";
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, 640, 480);

	while (!glfwWindowShouldClose(window))
	{	
		//handles window input
		processInput(window);

		//flushes queue events.
		glfwPollEvents();
	
		//swaps buffer to avoid tearing / sync issues that happens when displaying while drawing.
		glfwSwapBuffers(window);
		

	}

	





	glfwTerminate();

	
	return 0;

	system("pause");
}