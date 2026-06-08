#ifndef GBUFFER_H
#define GBUFFER_H

#include <glad/glad.h>

class Gbuffer {
    public:
        Gbuffer(unsigned int width, unsigned int height);
        ~Gbuffer();

        unsigned int FBO;
        unsigned int gPosition;
        unsigned int gNormal;
        unsigned int gAlbedo;
        unsigned int rboDepth;

        void bind();
        void unbind();
};

#endif
