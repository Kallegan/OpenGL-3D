#include "RectangleModel.h"


RectangleModel::RectangleModel(RectangleModelCreateInfo* createInfo)
{
	float l = createInfo->size.x / 1;
	float w = createInfo->size.y / 1;
	float h = createInfo->size.z / 1;
	
	vertices = {
	    -l, -w, -h, 0.0f, 0.0f, // bottom
		 l, -w, -h, 1.0f, 0.0f,
		 l,  w, -h, 1.0f, 1.0f,

		 l,  w, -h, 1.0f, 1,
		-l,  w, -h, 0.0f, 1,
		-l, -w, -h, 0.0f, 0,

		-l, -w,  h, 0.0f, 0.0f, //top
		 l, -w,  h, 1.0f, 0.0f,
		 l,  w,  h, 1.0f, 1.0f,

		 l,  w,  h, 1.0f, 1.0f,
		-l,  w,  h, 0.0f, 1.0f,
		-l, -w,  h, 0.0f, 0.0f,

		-l,  w,  h, 1.0f, 0.0f, //left
		-l,  w, -h, 1.0f, 1.0f,
		-l, -w, -h, 0.0f, 1.0f,

		-l, -w, -h, 0.0f, 1.0f,
		-l, -w,  h, 0.0f, 0.0f,
		-l,  w,  h, 1.0f, 0.0f,

		 l,  w,  h, 1.0f, 0.0f, //right
		 l,  w, -h, 1.0f, 1.0f,
		 l, -w, -h, 0.0f, 1.0f,

		 l, -w, -h, 0.0f, 1.0f,
		 l, -w,  h, 0.0f, 0.0f,
		 l,  w,  h, 1.0f, 0.0f,

		-l, -w, -h, 0.0f, 1.0f, //back
		 l, -w, -h, 1.0f, 1.0f,
		 l, -w,  h, 1.0f, 0.0f,

		 l, -w,  h, 1.0f, 0.0f,
		-l, -w,  h, 0.0f, 0.0f,
		-l, -w, -h, 0.0f, 1.0f,

		-l,  w, -h, 0.0f, 1.0f, //front
		 l,  w, -h, 1.0f, 1.0f,
		 l,  w,  h, 1.0f, 0.0f,

		 l,  w,  h, 1.0f, 0.0f,
		-l,  w,  h, 0.0f, 0.0f,
		-l,  w, -h, 0.0f, 1.0f
	};
	vertexCount = vertices.size() / 5;
	//int VBOSize = vertices.size() * sizeof(float); //how many bits to allocate in dynamic storage.
	//int stride = 5 * sizeof(float); //how many elements to get from one vertex to another in vector.

	//vertex buffer object, used as a set of data for the vertices.
	glCreateBuffers(1, &VBO);

	//(GL_DYNAMIC_STORAGE_BIT)allocates this amount of storage so we can change it later if needed.
	//vertices.data() gives adress to first element in vector.
	glNamedBufferStorage(VBO, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_STORAGE_BIT);
	//stores set of data and the instructions what the data represent. ex. what's vertices coordinates, RGB etc.

	glCreateVertexArrays(1, &VAO);
	//binds vertex array to vertex buffer.
	glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 5 * sizeof(float));

	//position
	//attribute pointers to give more info to OpenGL what the data means and where/how to access it.
	glEnableVertexArrayAttrib(VAO, 0);
	glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0); //0 since positions are in the start of vertex vector.
	glVertexArrayAttribBinding(VAO, 0, 0);

	//texture coordinate    location in shader
	glEnableVertexArrayAttrib(VAO, 1);					// floats behind first index(rgb).
	glVertexArrayAttribFormat(VAO, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
	glVertexArrayAttribBinding(VAO, 1, 0);
}

RectangleModel::~RectangleModel()
{
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}




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