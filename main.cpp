#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "stb_image.h"

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
		x, 1.0 - y flips the texture.
		 "\0" ends string.
	*/

	const char* vertexShaderSource = "#version 450 core\n"
		"layout (location = 0) in vec3 vertexPosition;\n"
		"layout (location = 1) in vec3 vertexColor;\n"
		"layout (location = 2) in vec2 vertexTexCoords;"
		"layout (location = 0) out vec3 fragmentColor;\n"
		"layout (location = 1) out vec2 fragmentTexCoords;"
		"void main()\n"
		"{\n"
		"    gl_Position = vec4(vertexPosition, 1.0);\n"
		"    fragmentColor = vertexColor;\n"
		"    fragmentTexCoords = vec2(vertexTexCoords.x, 1.0 - vertexTexCoords.y);\n"
		"}\0";

	//finalColor, 4d vector that adds another float at the end used for alpha.
	//multiply to color the texture.
	const char* fragmentShaderSource = "#version 450 core\n"
		"layout (location = 0) in vec3 fragmentColor;\n"
		"layout (location = 1) in vec2 fragmentTexCoords;\n"
		"uniform sampler2D basicTexture;\n"
		"out vec4 finalColor;\n"
		"void main()\n"
		"{\n"
		"    finalColor = vec4(fragmentColor, 1.0) * texture(basicTexture, fragmentTexCoords);\n"
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

	
	/* vpos    color     texcoords S=horizontal, 0-1, 0 = left, 1 right. T=vertical, 0 bottom, 1 top.
	X, Y, Z | R, G, B | S, T
	0 = centered.
	X = -0.25f a bit to the left, 0.25f right. 
	Y = positive up, negative down.
	Z = 0, middle of screen dept.

	R G B, (0 to 1) (1, 0, 0) colors vertex completly red etc.
	*/

	std::vector<float> vertices =  //using normalized device coordinates.
	{ 
    /*stride: 1.X     2.Y    3.Z   4.R   5.G   6.B	 7.S   8.T	 each colm = colm * sizeof(float) */
		     -0.25f, -0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,	 //vertice 1
		      0.25f, -0.25f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,	//vertice 2
		      0.0f,   0.25f, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f	   //vertice 3
	};
	int vertexCount = 3;
	int VBOSize = vertices.size() * sizeof(float); //how many bits to allocate in dynamic storage.
	int stride = 8 * sizeof(float); //how many elements to get from one vertex to another in vector.

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

	//position
	//attribute pointers to give more info to OpenGL what the data means and where/how to access it.
	glEnableVertexArrayAttrib(VAO, 0);
	glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0); //0 since positions are in the start of vertex vector.
	glVertexArrayAttribBinding(VAO, 0, 0);

	//color
	glEnableVertexArrayAttrib(VAO, 1);
	glVertexArrayAttribFormat(VAO, 1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float)); //3 floats behind first index.
	glVertexArrayAttribBinding(VAO, 1, 0);

	//texture coordinate
	glEnableVertexArrayAttrib(VAO, 2);
	glVertexArrayAttribFormat(VAO, 2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float)); //3 floats behind first index.
	glVertexArrayAttribBinding(VAO, 2, 0);

	//allocating texture 0 to the texture.
	glUniform1i(glGetUniformLocation(shader, "basicTexture"),0);

	//load image from project, get image details and set rgb+alpha.
	int texWidth, texHeight, channelCount;
	unsigned char* data = stbi_load("textures/wood.jpg", &texWidth, &texHeight, &channelCount, STBI_rgb_alpha);
	unsigned int texture;
	//create and store texture as 2d.
	glCreateTextures(GL_TEXTURE_2D, 1, &texture);
	glTextureStorage2D(texture, 1, GL_RGBA8, texWidth, texHeight);
	glTextureSubImage2D(texture, 0, 0, 0, texWidth, texHeight, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT); //if out of bound, repeate texture.
	glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT); 
	glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //if texture is far away, shrink using nearest.
	glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //if texture is far away, grow using linear.
	stbi_image_free(data); //free the data after setting all up.



	while (!glfwWindowShouldClose(window))
	{	
		//handles window input.
		processInput(window);

		//flushes queue events.
		glfwPollEvents();

		//draw
		glClearColor(0.2f, 0.2f, 0.3f, 1.0f); //clear screen.
		glClear(GL_COLOR_BUFFER_BIT); //clear buffer.
		glUseProgram(shader); //setup shader program.

		//binds to texture unit declared above with loaded texture.
		glBindTextureUnit(0, texture);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	
		//swaps buffer to avoid tearing / sync issues that happens when displaying while drawing.
		glfwSwapBuffers(window);

	}

	//free mem.
	glDeleteTextures(1, &texture);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteProgram(shader);
	glfwTerminate();	
	return 0;	
}