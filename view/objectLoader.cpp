#include "objectLoader.h"

std::vector<float> util::objLoadFromFile(const char* filename, glm::mat4 preTransform)
{
	std::vector<float> vertices;

	tinyobj::attrib_t attributes;
	std::vector<tinyobj::shape_t> shapes; //vector of all obj shapes.
	std::vector<tinyobj::material_t> materials; //vector of all material info.
	std::string warning, error;

	if (!tinyobj::LoadObj(&attributes, &shapes, &materials, &warning, &error, filename))
		std::cout << warning << error << '\n'; //prints error if attributes failes to get all its data.

	for (const auto& shape : shapes)
	{
		for (const auto& index : shape.mesh.indices)
		{
			//vec4 4by4 transfomation from vec3 of vertices to get the translation aswell.
			glm::vec4 pos =
			{
				attributes.vertices[3 * index.vertex_index],
				attributes.vertices[3 * index.vertex_index + 1],
				attributes.vertices[3 * index.vertex_index + 2],
				1 
			};

			pos = preTransform * pos;

			//normal we only need 3by3 since we are just interested in scaling and rotation normals.
			glm::vec3 normal =
			{
				attributes.normals[3 * index.normal_index],
				attributes.normals[3 * index.normal_index + 1],
				attributes.normals[3 * index.normal_index + 2]				
			};
			//normalize to allow scaling etc without throwing lighting.
			normal = glm::normalize(glm::mat3(preTransform) * normal); 


			glm::vec2 texCoord =
			{
				attributes.texcoords[2 * index.texcoord_index],
				attributes.texcoords[2 * index.texcoord_index + 1]			
			};

			vertices.push_back(pos.x);
			vertices.push_back(pos.y);
			vertices.push_back(pos.z);
			vertices.push_back(texCoord.x);
			vertices.push_back(texCoord.y);
			vertices.push_back(normal.x);
			vertices.push_back(normal.x);
			vertices.push_back(normal.x);
		}
	}

	return vertices;
}


