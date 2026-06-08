#ifndef TEXTURE
#define TEXTURE

#include <string>
#include <glad/glad.h>

class Texture {
    public:
        Texture(const std::string& textureFilePath);
        ~Texture();
        void bind(unsigned int unit);
        unsigned int ID;

    private:
        int width;
        int height;
        int numChannels;
};

#endif
