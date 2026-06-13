#include "renderer.h"
#include "camera.h"
#include "gbuffer.h"
#include "mesh.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <string>

Renderer::Renderer(
    const unsigned int screen_width,
    const unsigned int screen_height,
    const std::string& geoVertShader,
    const std::string& geoFragShader,
    const std::string& lightingVertShader,
    const std::string& lightingFragShader
) : aspectRatio((float) screen_width / (float) screen_height),
    gbuffer(screen_width, screen_height),
    geoShaders(geoVertShader, geoFragShader),
    lightingShaders(lightingVertShader, lightingFragShader),
    modelUniformLoc(glGetUniformLocation(geoShaders.ID, "model")),
    perspectiveUniformLoc(glGetUniformLocation(geoShaders.ID, "projection")),
    viewUniformLoc(glGetUniformLocation(geoShaders.ID, "view")),
    camPosUniformLoc(glGetUniformLocation(lightingShaders.ID, "cameraPos"))
{
    // Set up texture data
    geoShaders.use();
    int textureSamplerLoc = glGetUniformLocation(geoShaders.ID, "textureAlbedo");
    glUniform1i(textureSamplerLoc, 3);

    // Send lighting uniforms to lighting shader
    lightingShaders.use();
    int lightPosLoc         = glGetUniformLocation(lightingShaders.ID, "lightPos");
    int lightColorLoc       = glGetUniformLocation(lightingShaders.ID, "lightColor");
    int ambientStrengthLoc  = glGetUniformLocation(lightingShaders.ID, "ambientStrength");
    int diffuseStrengthLoc  = glGetUniformLocation(lightingShaders.ID, "diffuseStrength");
    int specularStrengthLoc = glGetUniformLocation(lightingShaders.ID, "specularStrength");
    int shininessLoc        = glGetUniformLocation(lightingShaders.ID, "shininess");

    // Send uniforms that don't change per frame here outside of the render loop
    glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(lightColorLoc, lightCol.x, lightCol.y, lightCol.z);
    glUniform1f(ambientStrengthLoc, phongAmbientStrength);
    glUniform1f(diffuseStrengthLoc, phongDiffuseStrength);
    glUniform1f(specularStrengthLoc, phongSpecularStrength);
    glUniform1f(shininessLoc, phongShininess);

    // Tell the lighting shader where to sample the geometry buffer textures from
    int gNormalSampleLoc   = glGetUniformLocation(lightingShaders.ID, "gNormal");
    int gPositionSampleLoc = glGetUniformLocation(lightingShaders.ID, "gPosition");
    int gAlbedoSampleLoc   = glGetUniformLocation(lightingShaders.ID, "gAlbedo");

    glUniform1i(gPositionSampleLoc, 0);
    glUniform1i(gNormalSampleLoc, 1);
    glUniform1i(gAlbedoSampleLoc, 2);
}

void Renderer::render(Mesh &mesh, Texture &texture, Camera &cam, glm::mat4 model) {
    // 1. Setup the geometry pass
    geoShaders.use();
    gbuffer.bind();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    texture.bind(3);

    // 1.1 Push the MVP matrices to the geo shader
    glm::mat4 projection = cam.getProjectionMatrix(aspectRatio);
    glm::mat4 view       = cam.getViewMatrix();

    glUniformMatrix4fv(
        modelUniformLoc,
        1,
        GL_FALSE,
        glm::value_ptr(model)
    );
    glUniformMatrix4fv(
        perspectiveUniformLoc,
        1,
        GL_FALSE,
        glm::value_ptr(projection)
    );
    glUniformMatrix4fv(
        viewUniformLoc,
        1,
        GL_FALSE,
        glm::value_ptr(view)
    );

    // 1.2 Bind the mesh VAO and draw the mesh
    glBindVertexArray(mesh.VAO);
    glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);

    // 1.3 Unbind geometry buffer
    gbuffer.unbind();

    // 2. Setup the lighitng pass
    lightingShaders.use();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 2.1 Bind the geometry buffer textures to the lighting pass
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gbuffer.gPosition);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gbuffer.gNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gbuffer.gAlbedo);

    // 3. Update camera position as neccessary
    glUniform3f(
        camPosUniformLoc,
        cam.position.x,
        cam.position.y,
        cam.position.z
    );

    // 4. Draw the fullscreen clip space lighting quad
    lightingQuad.draw();

}

void Renderer::setDirectionalLightProperties(glm::vec3 lightPosition, glm::vec3 lightColor) {
    // Update the class variables
    lightPos = lightPosition;
    lightCol = lightColor;

    // Get the unifrom locations
    lightingShaders.use();
    int lightPosLoc   = glGetUniformLocation(lightingShaders.ID, "lightPos");
    int lightColorLoc = glGetUniformLocation(lightingShaders.ID, "lightColor");

    // Set the uniforms to the updated values
    glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(lightColorLoc, lightCol.x, lightCol.y, lightCol.z);
}

void Renderer::setPhongMaterialProperties(
    float ambientStrength,
    float diffuseStrength,
    float specularStrength,
    float shininess
) {
    // Update the class variables
    phongAmbientStrength  = ambientStrength;
    phongDiffuseStrength  = diffuseStrength;
    phongSpecularStrength = specularStrength;
    phongShininess        = shininess;

    // Get the uniform locations
    lightingShaders.use();
    int ambientStrengthLoc  = glGetUniformLocation(lightingShaders.ID, "ambientStrength");
    int diffuseStrengthLoc  = glGetUniformLocation(lightingShaders.ID, "diffuseStrength");
    int specularStrengthLoc = glGetUniformLocation(lightingShaders.ID, "specularStrength");
    int shininessLoc        = glGetUniformLocation(lightingShaders.ID, "shininess");

    // Set the uniforms to the updated values
    glUniform1f(ambientStrengthLoc, phongAmbientStrength);
    glUniform1f(diffuseStrengthLoc, phongDiffuseStrength);
    glUniform1f(specularStrengthLoc, phongSpecularStrength);
    glUniform1f(shininessLoc, phongShininess);
}

Renderer::~Renderer() {

}
