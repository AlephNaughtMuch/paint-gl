#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <iostream>

#include "camera.h"
#include "framebuffer.h"
#include "input.h"
#include "mesh.h"
#include "obj_loader.h"
#include "renderer.h"
#include "texture.h"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Shaders
std::string geoVertexShaderFile      = "shaders/geometry.vert";
std::string geoFragShaderFile        = "shaders/geometry.frag";
std::string lightingVertShaderFile   = "shaders/lighting.vert";
std::string lightingFragShaderFile   = "shaders/lighting.frag";

// OBJ file
std::string objFilepath = "non_git_assets/damaged_helmet_source_glb.obj";

// Texture file
std::string textureFilePath = "non_git_assets/Material_MR.png";


// Helper functions
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

Camera setupCamera() {
    return Camera(
        glm::vec3(0,0,0),
        glm::vec3(0,1,0),
        glm::vec3(0,0,1)
    );
}

int main() {
    // Setup GLFWwindow
    GLFWwindow* window = setupGLFWwindow();

    // Setup Camera
    Camera cam =  setupCamera();

    // Setup model at origin
    glm::mat4 model = glm::mat4(1.0f);

    // Setup Texture
    Texture texture = Texture(textureFilePath);

    // Setup vertex data (and buffer(s)) and configure vertex attributes
    Mesh mesh = loadMeshFromObj(objFilepath);
    setupMesh(&mesh);

    // send camera as pointer
    glfwSetWindowUserPointer(window, &cam);

    // Setup Renderer
    Renderer renderer = Renderer(
        SCR_WIDTH,
        SCR_HEIGHT,
        geoVertexShaderFile,
        geoFragShaderFile,
        lightingVertShaderFile,
        lightingFragShaderFile
    );

    // delta time variables
    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;

    // Render loop
    while(!(bool) glfwWindowShouldClose(window)) {
        // calculate delta time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        // input
        processInput(window, deltaTime);

        // Add rotation to the model for viewing
        model = glm::rotate(
            glm::mat4(1.0f),
            (float) glfwGetTime() * glm::radians(45.0f),
            glm::vec3(0,1,0)
        );

        // render
        renderer.render(mesh, texture, cam, model);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
    }

    // glfw: terminate, clearing all previously allocated GLFW resources
    glfwTerminate();
    return 0;
}
