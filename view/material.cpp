#include "material.h"

Material::Material(MaterialCreateInfo* createInfo)
{
	//load image from project, get image details and set rgb+alpha.
	int texWidth, texHeight;
	image material = util::loadFromFile(createInfo->filename);
	unsigned char* data = material.pixels;
	texWidth = material.width;
	texHeight = material.height;	
	//create and store texture as 2d.
	glCreateTextures(GL_TEXTURE_2D, 1, &texture);
	glTextureStorage2D(texture, 1, GL_RGBA8, texWidth, texHeight);
	glTextureSubImage2D(texture, 0, 0, 0, texWidth, texHeight, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT); //if out of bound, repeate texture.
	glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //if texture is far away, shrink using nearest.
	glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //if texture is far away, grow using linear.
	util::freeImgMem(material);
}

Material::~Material()
{
	glDeleteTextures(1, &texture);
}

void Material::use()
{
	glBindTextureUnit(0, texture);
}
