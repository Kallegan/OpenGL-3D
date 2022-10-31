#include "config.h"
#include "view/shader.h"
#include "model/Cube.h"
#include "model/player.h"

void processInput(GLFWwindow* window, Player* player, int halfWidht, int halfHeight)
{
	int wasdState{ 0 };
	float walkDirection{ player->eulers.z};
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
		player->position += moveSpeed * glm::vec3
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
	player->eulers.z -= deltaX * mouseSpeed;

	//vertical rotation
	float deltaY{ static_cast<float>(mouseY - halfHeight) };
	//sets y rotation to clamp up / down based on look angle.
	player->eulers.y = std::max(std::min(player->eulers.y + deltaY * mouseSpeed, 179.0f), 1.0f);
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

	//create playerinfo class.
	PlayerCreateInfo playerInfo;
	//pass data to playerinfo.
	playerInfo.eulers = { 0.0f, 90.0f, 0.0f };
	playerInfo.position = { 0.0f, 0.0f, 0.0f };
	//create new player with the data from current playerinfo.
	Player* player = new Player(&playerInfo);

	CubeCreateInfo cubeInfo;
	cubeInfo.eulers = { 0.0f, 0.0f, 0.0f };
	cubeInfo.positions = { 3.0f, 0.0f, 0.5f };
	Cube* cube = new Cube(&cubeInfo);


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
		//handles window input.
		processInput(window, player, halfWidth, halfHeight);
		//flushes queue events.
		glfwPollEvents();
		
		player->update();
		cube->update(1.0f);
		
		//prepare shaders
		glm::mat4 viewTransform { glm::lookAt(
			player->position, 
			player->position + player->forwards,
			player->up) };
		//set view in shader
		glUniformMatrix4fv(glGetUniformLocation(shader, "view"),
			1, GL_FALSE, glm::value_ptr(viewTransform));		

		//update transform
		glm::mat4 modelTransform = { glm::mat4(1.0f) };
		modelTransform = glm::translate(modelTransform, cube->position);

		modelTransform = modelTransform * glm::eulerAngleXYZ
		(
			cube->eulers.x, cube->eulers.y, cube->eulers.z
		);		

		glUniformMatrix4fv(glGetUniformLocation(shader, "model"),
			1, GL_FALSE, glm::value_ptr(modelTransform));


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