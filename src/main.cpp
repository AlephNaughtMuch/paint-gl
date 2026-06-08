#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "camera.h"
#include "framebuffer.h"
#include "gbuffer.h"
#include "input.h"
#include "mesh.h"
#include "obj_loader.h"
#include "quad.h"
#include "shader.h"
#include "texture.h"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Shaders
std::string defaulatVertexShaderFile = "shaders/default.vert";
std::string defaulatFragShaderFile   = "shaders/default.frag";
std::string geoVertexShaderFile      = "shaders/geometry.vert";
std::string geoFragShaderFile        = "shaders/geometry.frag";
std::string lightingVertShaderFile   = "shaders/lighting.vert";
std::string lightingFragShaderFile   = "shaders/lighting.frag";

// OBJ file
std::string objFilepath = "non_git_assets/damaged_helmet_source_glb.obj";

// Texture file
std::string textureFilePath = "non_git_assets/Material_MR.png";


// Helper functions
void setupShaders(
    Shader &geoShaders,
    Shader &lightingShaders,
    glm::vec3 lightPos,
    glm::vec3 lightColor,
    float ambientStrength,
    float diffuseStrength,
    float specularStrength,
    float shininess,
    int &modelLoc,
    int &perspectiveLoc,
    int &viewLoc,
    int &camPosLoc
) {
    // build and compile our shader program

    // Send MVP matrices to geo shader
    geoShaders.use();
    modelLoc       = glGetUniformLocation(geoShaders.ID, "model");
    perspectiveLoc = glGetUniformLocation(geoShaders.ID, "projection");
    viewLoc        = glGetUniformLocation(geoShaders.ID, "view");

    // set up texture data
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
    camPosLoc               = glGetUniformLocation(lightingShaders.ID, "cameraPos");

    // send uniforms that don't change per frame here outside of the render loop
    glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(lightColorLoc, lightColor.x, lightColor.y, lightColor.z);
    glUniform1f(ambientStrengthLoc, ambientStrength);
    glUniform1f(diffuseStrengthLoc, diffuseStrength);
    glUniform1f(specularStrengthLoc, specularStrength);
    glUniform1f(shininessLoc, shininess);



}

GLFWwindow* setupGLFWwindow() {
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(
        SCR_WIDTH,
        SCR_HEIGHT,
        "paint-gl",
        NULL,
        NULL
    );

    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // glfw: Input mode
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);

    // glad: load all OpenGL function pointers
    if (!(bool) gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
    glEnable(GL_DEPTH_TEST);

    // Enable backface culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    return window;
}

int main() {
    // setup glfw window
    GLFWwindow* window = setupGLFWwindow();

    // Create geometry buffer
    Gbuffer gBuffer = Gbuffer(SCR_WIDTH, SCR_HEIGHT);


    // Create MVP matrices
    Camera cam = Camera(
        glm::vec3(0,0,0),
        glm::vec3(0,1,0),
        glm::vec3(0,0,1)
    );

    glm::mat4 model = glm::mat4(1.0f);

    // set up shaders and retrieve locations
    Shader geoShaders =  Shader(geoVertexShaderFile, geoFragShaderFile);
    Shader lightingShaders = Shader(lightingVertShaderFile, lightingFragShaderFile);


    int modelLoc, perspectiveLoc, viewLoc, camPosLoc;
    glm::vec3 lightPos     = {-2 , 5, 1};
    glm::vec3 lightColor   = {1, 1, 1};
    float ambientStrength  = 0.2f;
    float diffuseStrength  = 0.5f;
    float specularStrength = 0.8f;
    float shininess        = 32.0f;

    setupShaders(
        geoShaders,
        lightingShaders,
        lightPos,
        lightColor,
        ambientStrength,
        diffuseStrength,
        specularStrength,
        shininess,
        modelLoc,
        perspectiveLoc,
        viewLoc,
        camPosLoc
    );

    // Texture
    Texture texture = Texture(textureFilePath);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    Mesh mesh = loadMeshFromObj(objFilepath);
    setupMesh(&mesh);

    // delta time variables
    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;

    // send camera as pointer
    glfwSetWindowUserPointer(window, &cam);

    // Setup lighting pass quad
    Quad lightingQuad = Quad();



    // Use the textures
    geoShaders.use();
    lightingShaders.use();


    // tell the lighting shader where to sample from
    int gNormalSampleLoc = glGetUniformLocation(lightingShaders.ID, "gNormal");
    int gPositionSampleLoc = glGetUniformLocation(lightingShaders.ID, "gPosition");
    int gAlbedoSampleLoc = glGetUniformLocation(lightingShaders.ID, "gAlbedo");
    glUniform1i(gPositionSampleLoc, 0);
    glUniform1i(gNormalSampleLoc, 1);
    glUniform1i(gAlbedoSampleLoc, 2);


    // Render loop
    while(!(bool) glfwWindowShouldClose(window)) {
        // calculate delta time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        // input
        processInput(window, deltaTime);

        // 1. geometry pass
        geoShaders.use();
        gBuffer.bind();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        texture.bind(3);

        // Push the MVP matrices to the geo shader
        model = glm::rotate(glm::mat4(1.0f), (float) glfwGetTime() * glm::radians(45.0f), glm::vec3(0,1,0));
        glm::mat4 projection = cam.getProjectionMatrix((float) SCR_WIDTH / (float) SCR_HEIGHT);
        glm::mat4 view = cam.getViewMatrix();

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(perspectiveLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        // bind the mesh VAO and draw the mesh
        glBindVertexArray(mesh.VAO);
        glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);

        // unbind geometry buffer
        gBuffer.unbind();

        // 2. lighting pass
        lightingShaders.use();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Bind the geometry buffer textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gBuffer.gPosition);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gBuffer.gNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, gBuffer.gAlbedo);

        glUniform3f(camPosLoc, cam.position.x, cam.position.y, cam.position.z);

        // Draw the fullscreen clip space quad
        lightingQuad.draw();


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
    }

    // glfw: terminate, clearing all previously allocated GLFW resources
    glfwTerminate();
    return 0;
}
