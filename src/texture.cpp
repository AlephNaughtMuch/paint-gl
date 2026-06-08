#include "texture.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const std::string& textureFilePath) {
    // Generate and bind the texture handle
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    // Set the texture wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load and generate the texture
    const char* filePath = textureFilePath.c_str();
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filePath, &width, &height, &numChannels, 0);
    GLenum format = numChannels == 4 ? GL_RGBA : GL_RGB;
    if (data) {
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            format,
            width,
            height,
            0,
            format,
            GL_UNSIGNED_BYTE,
            data
        );
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture, using white fallback!" << std::endl;
        unsigned char whitePixel[] = {255,255,255,255};
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            1,
            1,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            whitePixel
        );
    }
    stbi_image_free(data);
}

Texture::~Texture() {
    glDeleteTextures(1, &ID);
}

void Texture::bind(unsigned int unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, ID);
}
