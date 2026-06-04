#include "input.h"
#include "camera.h"
#include <GLFW/glfw3.h>

void processInput(GLFWwindow* window, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (
        glfwGetKey(window, GLFW_KEY_W)    == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_S)    == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_A)    == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_D)    == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_UP)   == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS
   ) {
        CameraMovement direction;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            direction = CameraMovement::FORWARD;
        } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            direction = CameraMovement::BACKWARD;
        } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            direction = CameraMovement::LEFT;
        } else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            direction = CameraMovement::RIGHT;
        } else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            direction = CameraMovement::UP;
        } else {
            direction = CameraMovement::DOWN;
        }

        Camera* cam = (Camera*) glfwGetWindowUserPointer(window);
        cam->processKeyboard(direction, deltaTime);
    }
}

void mouseCallback(GLFWwindow *window, double xPos, double yPos) {
    static double lastX = 400;
    static double lastY = 300;
    static bool firstMouse = true;

    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
        return;
    }

    double xOffset = xPos - lastX;
    double yOffset = yPos - lastY;

    lastX = xPos;
    lastY = yPos;

    Camera* cam = (Camera*) glfwGetWindowUserPointer(window);
    cam->processMouse(xOffset, yOffset);
}
