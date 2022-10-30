#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
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

unsigned int createShader() 
{

	/*  vertexPositions, 3d vector(x, y, z), coordiates of the vertesies.
		vertexColor, 3d vector(rgb), telling shader what colors the vertesies has.
		fragmentColor, -||-, information for fragment shader.
		gl_Positions, global 4d vector, gets passed vertexPositions. 4th vector(w) is perspective.
		use main() to pass data. pos to global and fragmentcolor to vertex shader. fragment shader doesn't know pos.
		 "\0" ends string.
	*/

	const char* vertexShaderSource = "#version 450 core\n"
		"layout (location = 0) in vec3 vertexPosition;\n"
		"layout (location = 1) in vec3 vertexColor;\n"
		"layout (location = 0) out vec3 fragmentColor;\n"
		"void main()\n"
		"{\n"
		"    gl_Position = vec4(vertexPosition, 1.0);\n"
		"    fragmentColor = vertexColor;\n"
		"}\0";

	//finalColor, 4d vector that adds another float at the end used for alpha.
	const char* fragmentShaderSource = "#version 450 core\n"
		"layout (location = 0) in vec3 fragmentColor;\n"
		"out vec4 finalColor;\n"
		"void main()\n"
		"{\n"
		"    finalColor = vec4(fragmentColor, 1.0);\n"
		"}\0";

	//stores index of the created memory allocation for the shaders.
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//pass the allocated adress for created shader, count, source code adress.
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//compiles the shader inside the OpenGL data structure.
	glCompileShader(vertexShader);
	int success;
	char errorLog[1024];
	//queery the error msg queue and prints the errors if compile isn't success.
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 1024, NULL, errorLog);
		std::cout << "VertexShader compile error: \n" << errorLog << '\n';
	}


	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 1024, NULL, errorLog);
		std::cout << "FragmentShader compile error: \n" << errorLog << '\n';
	}

	//creating a shader program and storing memory adress.
	unsigned int shader = glCreateProgram();
	//binding my shaders to the shader program.
	glAttachShader(shader, vertexShader);
	glAttachShader(shader, fragmentShader);
	//linking shaders together.
	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 1024, NULL, errorLog);
		std::cout << "Shader linking error: \n" << errorLog << '\n';
	}

	//after linking the shaders, the exists inside the program so they can be deleted.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shader;
}


int main()
{	
	int width = 640;
	int height = 480;

	

	GLFWwindow* window = InitWindow(width, height);	
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	
	unsigned int shader = createShader();
	//set shader as active.
	glUseProgram(shader);

	while (!glfwWindowShouldClose(window))
	{	
		//handles window input
		processInput(window);

		//flushes queue events.
		glfwPollEvents();
	
		//swaps buffer to avoid tearing / sync issues that happens when displaying while drawing.
		glfwSwapBuffers(window);

	}

	glDeleteProgram(shader);
	glfwTerminate();	
	return 0;	
}