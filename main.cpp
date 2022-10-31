#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "stb_image.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void processInput(GLFWwindow* window, glm::vec3& playerPosition, glm::vec3& playerEulers, int halfWidht, int halfHeight)
{
	int wasdState{ 0 };
	float walkDirection{ playerEulers.z };
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
		playerPosition += moveSpeed * glm::vec3
		{
			glm::cos(glm::radians(walkDirection)),
			glm::sin(glm::radians(walkDirection)),
			0.0f
		};
	}

	//mouse controller
	double mouseX, mouseY;
	float mouseSpeed = 0.1f;
	//passing ref so cursorpos can be set from glfw function.
	glfwGetCursorPos(window, &mouseX, &mouseY);
	glfwSetCursorPos(window, halfWidht, halfHeight);

	//horizontal rotation
	float deltaX{ static_cast<float>(mouseX - halfWidht) };
	playerEulers.z -= deltaX * mouseSpeed;

	//vertical rotation
	float deltaY{ static_cast<float>(mouseY - halfHeight) };
	//sets y rotation to clamp up / down based on look angle.
	playerEulers.y = std::max(std::min(playerEulers.y + deltaY * mouseSpeed, 179.0f), 1.0f);
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
		gl_Positions, global 4d vector, gets passed vertexPositions. 4th vector(w) is perspective.
		uniform mat4, XYZ + 1. Last dimension used for translations. 
		x, 1.0 - y flips the texture.
		 "\0" ends string.
	*/

	const char* vertexShaderSource = "#version 450 core\n"
		"layout (location = 0) in vec3 vertexPosition;\n"
		"layout (location = 1) in vec2 vertexTexCoords;\n"		
		"layout (location = 0) out vec2 fragmentTexCoords;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"void main()\n"
		"{\n"
		"    gl_Position = projection * view * model * vec4(vertexPosition, 1.0);\n"
		"    fragmentTexCoords = vec2(vertexTexCoords.x, 1.0 - vertexTexCoords.y);\n"
		"}\0";

	const char* fragmentShaderSource = "#version 450 core\n"
		"layout (location = 0) in vec2 fragmentTexCoords;\n"
		"uniform sampler2D basicTexture;\n"
		"out vec4 finalColor;\n"
		"void main()\n"
		"{\n"
		"    finalColor = texture(basicTexture, fragmentTexCoords);\n"
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
	glm::vec3 playerPosition = { 0.0f, 0.0f, 0.0f };
	glm::vec3 playerEulers = { 0.0f, 90.0f, 0.0f };
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
	
	unsigned int shader = createShader();
	glUseProgram(shader);

	//2d
	/* vpos    color     texcoords S=horizontal, 0-1, 0 = left, 1 right. T=vertical, 0 bottom, 1 top.
	X, Y, Z | R, G, B | S, T
	0 = centered.
	X = -0.25f a bit to the left, 0.25f right. 
	Y = positive up, negative down.
	Z = 0, middle of screen dept.

	R G B, (0 to 1) (1, 0, 0) colors vertex completly red etc.
	

	std::vector<float> vertices =  //using normalized device coordinates.
	{ 
    //stride: 1.X     2.Y    3.Z   4.R   5.G   6.B	 7.S   8.T	 each colm = colm * sizeof(float) 
		     -0.25f, -0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,	 //vertice 1
		      0.25f, -0.25f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,	//vertice 2
		      0.0f,   0.25f, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f	   //vertice 3
	};
	int vertexCount = vertices.size()/8;
	int VBOSize = vertices.size() * sizeof(float); //how many bits to allocate in dynamic storage.
	int stride = 8 * sizeof(float); //how many elements to get from one vertex to another in vector.
	*/

	//CUBE//
	//x,y,z,s,t
	std::vector<float> vertices = {
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // bottom
			 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,

			 0.5f,  0.5f, -0.5f, 1.0f, 1,0.f
			-0.5f,  0.5f, -0.5f, 0.0f, 1,0.f
			-0.5f, -0.5f, -0.5f, 0.0f, 0,0.f

			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, //top
			 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,

			 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, //left
			-0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, //right
			 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

			 0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, //back
			 0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,

			 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, //front
			 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, 0.0f, 1.0f
	};

	int vertexCount = vertices.size() / 5;
	int VBOSize = vertices.size() * sizeof(float); //how many bits to allocate in dynamic storage.
	int stride = 5 * sizeof(float); //how many elements to get from one vertex to another in vector.

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

	//texture coordinate    location in shader
	glEnableVertexArrayAttrib(VAO, 1);					// floats behind first index(rgb).
	glVertexArrayAttribFormat(VAO, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
	glVertexArrayAttribBinding(VAO, 1, 0);

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
	glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //if texture is far away, shrink using nearest.
	glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //if texture is far away, grow using linear.
	stbi_image_free(data); //free the data after setting all up.


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
		//player forward vector
		glm::vec3 forwards
		{
			glm::sin(glm::radians(playerEulers.y)) * glm::cos(glm::radians(playerEulers.z)),
			glm::sin(glm::radians(playerEulers.y)) * glm::sin(glm::radians(playerEulers.z)),
			glm::cos(glm::radians(playerEulers.y))
		};

		glm::vec3 globalUp{ 0.0f, 0.0f, 1.0 };
		glm::vec3 right{ glm::cross(forwards, globalUp) };
		glm::vec3 up{ glm::cross(right, forwards) };		
		//update player camera view
		glm::mat4 viewTransform{ glm::lookAt(playerPosition, playerPosition + forwards, up) };
		glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(viewTransform));


		//handles window input.
		processInput(window, playerPosition, playerEulers, halfWidth, halfHeight);

		//flushes queue events.
		glfwPollEvents();

		//update transform
		float angle = glm::radians(static_cast<float>(10 * glfwGetTime()));
		glm::mat4 modelTransform = glm::mat4(1.0f);
		modelTransform = glm::translate(modelTransform, { 3.0f, 0.0f, 0.0f });
		//modelTransform = glm::rotate(modelTransform, angle, { 1.0f, 1.0f, 0.0f });
		//modelTransform = glm::rotate(modelTransform, 1.0f * angle, { 0.0f, 1.0f, 0.0f });
		glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, false, glm::value_ptr(modelTransform));


		//draw		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear buffer.
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