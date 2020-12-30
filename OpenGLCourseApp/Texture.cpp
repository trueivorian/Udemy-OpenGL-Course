#include "Texture.h"

Texture::Texture() : textureID(0), width(0), height(0), bitDepth(0), fileLocation('\0') {}

Texture::Texture(char* fileLoc) : textureID(0), width(0), height(0), bitDepth(0), fileLocation(fileLoc) {}

void Texture::LoadTexture() {
	unsigned char* texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0); // 1 char = 1 byte images and text are commonly stored in char/byte arrays
	if (!texData) {
		printf("failed to find: %s\n", fileLocation);
		return;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Blends pixels when we zoom in
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Blends pixels when we zoom out

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData); // Loads data from the image
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(texData);
}

void Texture::UseTexture() {
	// When the texture is being run in the fragment shader,
	// the sampler is given access to texture data through the texture unit.
	// We could bind multiple textures to the same object and access the relevant textures in 
	// the fragment shader (this would require creating a uniform variable
	// for each individual texture in the shader).
	glActiveTexture(GL_TEXTURE0); // GL_TEXTURE0 is one of up to 16 (sometimes 32) texture units 
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::ClearTexture() {
	glDeleteTextures(1, &textureID);
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 
}