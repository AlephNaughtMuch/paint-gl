#include "camera.h"

#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// Constructor
Camera::Camera(glm::vec3 pos, glm::vec3 upVector, glm::vec3 fwd) {
    // Set camera properties
    position = pos;
    forward = fwd;
    up = upVector;

    updateVectors();
}

void Camera::updateVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward = glm::normalize(front);
    right = glm::normalize(glm::cross(forward, up));
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(position, position + forward, up);
}

glm::mat4 Camera::getProjectionMatrix(float aspectRatio) {
    return glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar);
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime) {
    if (direction == CameraMovement::FORWARD) {
        position += forward * speed * deltaTime;
    } else if (direction == CameraMovement::BACKWARD) {
        position -= forward * speed * deltaTime;
    } else if (direction == CameraMovement:: LEFT) {
        position -= right * speed * deltaTime;
    } else if (direction == CameraMovement::RIGHT) {
        position += right * speed * deltaTime;
    } else if (direction == CameraMovement::UP) {
        position.y += speed * deltaTime;
    } else if (direction == CameraMovement::DOWN) {
        position.y -= speed * deltaTime;
    }
}

void Camera::processMouse(float xOffset, float yOffset) {
    yaw += xOffset * sensitivity;
    pitch += yOffset * sensitivity;

    pitch = std::min(pitch, 89.0f);
    pitch = std::max(pitch, -89.0f);
    // clamp pitch
    updateVectors();
}
