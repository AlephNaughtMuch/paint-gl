#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
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

    // Create MVP matrices
    Camera cam = Camera(
        glm::vec3(0,0,0),
        glm::vec3(0,1,0),
        glm::vec3(0,0,1)
    );

    glm::mat4 model = glm::mat4(1.0f);

    // build and compile our shader program
    Shader shaders = Shader(vertexShaderFile, fragShaderFile);

    // Send MVP matrices to shader
    int modelLoc = glGetUniformLocation(shaders.ID, "model");
    int perspectiveLoc = glGetUniformLocation(shaders.ID, "projection");
    int viewLoc = glGetUniformLocation(shaders.ID, "view");


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
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = cam.getProjectionMatrix((float) SCR_WIDTH / (float) SCR_HEIGHT);
        glm::mat4 view = cam.getViewMatrix();

        shaders.use();
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
