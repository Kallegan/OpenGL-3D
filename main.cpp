#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

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
	glUseProgram(shader);

	
	/* 
	X, Y, Z | R, G, B 
	0 = centered.
	X = -0.25f a bit to the left, 0.25f right. 
	Y = positive up, negative down.
	Z = 0, middle of screen dept.

	R G B, (0 to 1) 1, 0 ,0 colors vertex completly red etc.
	*/

	std::vector<float> vertices =  //using normalized device coordinates.
	{ 
    /*stride:   1       2     3     4     5     6     each colm = colm * sizeof(float) */
		     -0.25f, -0.25f, 0.0f, 1.0f, 0.0f, 0.0f,  //vertice 1
		      0.25f, -0.25f, 0.0f, 0.0f, 1.0f, 0.0f,  //vertice 2
		     -0.0f,   0.25f, 0.0f, 0.0f, 0.0f, 1.0f   //vertice 3
	};
	int vertexCount = 3;
	int VBOSize = vertices.size() * sizeof(float); //how many bits to allocate in dynamic storage.
	int stride = 6 * sizeof(float); //how many elements to get from one vertex to another in vector.

	//vertex buffer object, used as a set of data for the vertices.
	unsigned int VBO;
	glCreateBuffers(1, &VBO);

	//(GL_DYNAMIC_STORAGE_BIT)allocates this amount of storage so we can change it later if needed.
	//vertices.data() gives adress to first element in vector.
	glNamedBufferStorage(VBO, VBOSize, vertices.data(), GL_DYNAMIC_STORAGE_BIT);

	//stores set of data and the instructions what the data represent. ex. what's vertices coordinates, RGB etc.
	unsigned int VAO;
	glCreateVertexArrays(1, &VAO);
	//binds vertex array to vertex buffer.
	glVertexArrayVertexBuffer(VAO, 0, VBO, 0, stride);

	//attribute pointers to give more info to OpenGL what the data means and where/how to access it.
	glEnableVertexArrayAttrib(VAO, 0);
	glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0); //0 since positions are in the start of vertex vector.
	glVertexArrayAttribBinding(VAO, 0, 0);

	glEnableVertexArrayAttrib(VAO, 1);
	glVertexArrayAttribFormat(VAO, 1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float)); //3 floats behind first index.
	glVertexArrayAttribBinding(VAO, 1, 0);



	while (!glfwWindowShouldClose(window))
	{	
		//handles window input
		processInput(window);

		//flushes queue events.
		glfwPollEvents();

		//draw
		glClearColor(0.5f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shader);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	
		//swaps buffer to avoid tearing / sync issues that happens when displaying while drawing.
		glfwSwapBuffers(window);

	}

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteProgram(shader);
	glfwTerminate();	
	return 0;	
}