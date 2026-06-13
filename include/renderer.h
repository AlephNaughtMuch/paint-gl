#ifndef RENDERER_H
#define RENDERER_H

#include "camera.h"
#include "gbuffer.h"
#include "mesh.h"
#include "quad.h"
#include "shader.h"
#include "texture.h"
#include <glm/glm.hpp>
#include <string>

class Renderer {
    public:
        Renderer(
            const unsigned int screen_width,
            const unsigned int screen_height,
            const std::string& geoVertShader,
            const std::string& geoFragShader,
            const std::string& lightingVertShader,
            const std::string& lightingFragShader
        );
        void render(
            Mesh& mesh,
            Texture& texture,
            Camera& cam,
            glm::mat4 model
        );
        void setDirectionalLightProperties(
            glm::vec3 lightPosition,
            glm::vec3 lightColor
        );
        void setPhongMaterialProperties(
            float ambientStrength,
            float diffuseStrength,
            float specularStrength,
            float shininess
        );
        ~Renderer();

    private:
        float   aspectRatio;

        Gbuffer gbuffer;
        Shader  geoShaders;
        Shader  lightingShaders;
        Quad    lightingQuad;

        int modelUniformLoc;
        int perspectiveUniformLoc;
        int viewUniformLoc;
        int camPosUniformLoc;

        glm::vec3 lightPos = glm::vec3(0, 5, 0);
        glm::vec3 lightCol = glm::vec3(1, 1, 1);

        float phongAmbientStrength  = 0.2f;
        float phongDiffuseStrength  = 0.5f;
        float phongSpecularStrength = 0.5f;
        float phongShininess        = 32.0f;

};

#endif
