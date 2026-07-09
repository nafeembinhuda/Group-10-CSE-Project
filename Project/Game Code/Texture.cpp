#include "Texture.h"

Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
{
	type = texType;
	int widthImg, heightImg, numColCh;
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 4);

	if (!bytes)
	{
		std::cout << "Failed to load image: " << image << "\n";
		std::cout << "STB reason: " << stbi_failure_reason() << "\n";
		return;
	}

	glGenTextures(1, &ID);

	glActiveTexture(slot);
	glBindTexture(texType, ID);

	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(texType, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
	glGenerateMipmap(texType);

	stbi_image_free(bytes);
	glBindTexture(texType, 0);
}

void Texture::texUnit(Shader shader, const char* uniform, GLuint unit)
{
	GLuint spriteUni = glGetUniformLocation(shader.ID, uniform);
	shader.Activate();
	glUniform1i(spriteUni, unit);
}

void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0); 
	glBindTexture(type, ID);
}

void Texture::Unbind()
{
	glBindTexture(type, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}