#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>

#include "camera.h"
#include "framebuffer.h"
#include "input.h"
#include "mesh.h"
#include "obj_loader.h"
#include "shader.h"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Shaders
std::string vertexShaderFile = "shaders/default.vert";
std::string fragShaderFile   = "shaders/default.frag";

// OBJ file
std::string objFilepath = "assets/bunny.obj";

int main() {

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
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // glfw: Input mode
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);

    // glad: load all OpenGL function pointers
    if (!(bool) gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);

    // Enable backface culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Create MVP matrices
    Camera cam = Camera(
        glm::vec3(0,0,0),
        glm::vec3(0,1,0),
        glm::vec3(0,0,1)
    );

    glm::mat4 model = glm::mat4(1.0f);

    // build and compile our shader program
    Shader shaders = Shader(vertexShaderFile, fragShaderFile);
    shaders.use();

    // Send MVP matrices to shader
    int modelLoc       = glGetUniformLocation(shaders.ID, "model");
    int perspectiveLoc = glGetUniformLocation(shaders.ID, "projection");
    int viewLoc        = glGetUniformLocation(shaders.ID, "view");

    // establish and send Phong uniforms to shader
    glm::vec3 lightPos     = glm::vec3(-2, 5, 1);
    glm::vec3 lightColor   = glm::vec3(1, 1, 1);
    float ambientStrength  = 0.2f;
    float diffuseStrength  = 1.0f;
    float specularStrength = 0.5f;
    float shininess        = 32.0f;

    int lightPosLoc         = glGetUniformLocation(shaders.ID, "lightPos");
    int camPosLoc           = glGetUniformLocation(shaders.ID, "cameraPos");
    int lightColorLoc       = glGetUniformLocation(shaders.ID, "lightColor");
    int ambientStrengthLoc  = glGetUniformLocation(shaders.ID, "ambientStrength");
    int diffuseStrengthLoc  = glGetUniformLocation(shaders.ID, "diffuseStrength");
    int specularStrengthLoc = glGetUniformLocation(shaders.ID, "specularStrength");
    int shininessLoc        = glGetUniformLocation(shaders.ID, "shininess");

    // send uniforms that don't change per frame here outside of the render loop
    glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(lightColorLoc, lightColor.x, lightColor.y, lightColor.z);
    glUniform1f(ambientStrengthLoc, ambientStrength);
    glUniform1f(diffuseStrengthLoc, diffuseStrength);
    glUniform1f(specularStrengthLoc, specularStrength);
    glUniform1f(shininessLoc, shininess);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    Mesh mesh = loadMeshFromObj(objFilepath);
    setupMesh(&mesh);

    // delta time variables
    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;

    // send camera as pointer
    glfwSetWindowUserPointer(window, &cam);

    // Render loop
    while(!(bool) glfwWindowShouldClose(window)) {
        // calculate delta time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        // input
        processInput(window, deltaTime);

        // render
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        model = glm::rotate(model, glm::radians(0.5f), glm::vec3(0,1,0));
        glm::mat4 projection = cam.getProjectionMatrix((float) SCR_WIDTH / (float) SCR_HEIGHT);
        glm::mat4 view = cam.getViewMatrix();

        glUniform3f(camPosLoc, cam.position.x, cam.position.y, cam.position.z);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(perspectiveLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        glBindVertexArray(mesh.VAO);
        glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
    }

    // glfw: terminate, clearing all previously allocated GLFW resources
    glfwTerminate();
    return 0;
}
