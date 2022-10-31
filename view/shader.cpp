#include "shader.h"

unsigned int util::loadShader(const char* vertexFilepath, const char* fragmentfilepath)
{
	std::ifstream fileReader;
	std::stringstream bufferedLines;
	std::string line;

	//using filereader and stringstream to feed the lines and store them.
	fileReader.open(vertexFilepath);
	while (std::getline(fileReader, line))	
		bufferedLines << line << '\n';	

	//stores the shader as a string, converting to char pointer to be used as source.
	std::string vertexShaderSource = bufferedLines.str();
	const char* vertexSrc = vertexShaderSource.c_str();
	bufferedLines.str("");
	fileReader.close();


	fileReader.open(fragmentfilepath);
	while (std::getline(fileReader, line))	
		bufferedLines << line << '\n';	

	std::string fragmentShaderSource = bufferedLines.str();
	const char* fragmentSrc = fragmentShaderSource.c_str();
	bufferedLines.str("");
	fileReader.close();	

	//stores index of the created memory allocation for the shaders.
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//pass the allocated adress for created shader, count, source code adress.
	glShaderSource(vertexShader, 1, &vertexSrc, NULL);
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
	glShaderSource(fragmentShader, 1, &fragmentSrc, NULL);
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
